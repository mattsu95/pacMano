//Arthur Gomes Pivotto && Matheus Seghatti de Moraes
//m_ esta indicando membros da classe para separalos de variaveis locais


#include "Astar.h"
#include <cmath>
#include <chrono>
#include <limits>

AstarAlgoritmo::AstarAlgoritmo() { //construtor monta o grid com base no mapa do jogo localizado no Game.h
    for (int linha{}; linha < LINHAS; linha++) {
        for (int coluna{}; coluna < COLUNAS; coluna++) {
            m_grid[linha][coluna].linha = linha; //define a coordenada da linha do node
            m_grid[linha][coluna].coluna = coluna; //define a coordenada da coluna do node
            m_grid[linha][coluna].is_obstacle = (mapa[linha][coluna] == 1); //no mapa 1 = obstaculo e 0 = caminho, aqui e checado isso
        }
    }
}

float AstarAlgoritmo::heuristica(const Node* a, const Node* b) {    //heuristica Manhattan soma da diferenca absoluta de linha e coluna
    return static_cast<float>(std::abs(a->linha - b->linha) + std::abs(a->coluna - b->coluna)); //retorna o manhattan como float
}

void AstarAlgoritmo::resetarGrid() { //funcao pra reinicializar os dados de busca
    for (int linha{}; linha < LINHAS; linha++) {
        for (int coluna{}; coluna < COLUNAS; coluna++) {
            Node& node = m_grid[linha][coluna];
            node.g_cost = std::numeric_limits<float>::max(); //custo do inicio ate o node atual, numero maximo do float ja que nao se conhece o caminho
            node.f_cost = std::numeric_limits<float>::max(); //F = G + H
            node.h_cost = {};  //estimativa ate o final
            node.visitado = false;
            node.parent = nullptr;
        }
    }
}

void AstarAlgoritmo::reconstruirCaminho(Node* nodeFinal) { //conta quantos passos tem indo do parent do node final ate o inicial pra descobrir total de nodes
    int passos{};
    Node* atual = nodeFinal;
    while (atual != nullptr) { //node inicial nao tem parent
        passos++;
        atual = atual->parent;
    }

    m_tamanhoCaminho = passos;
    atual = nodeFinal;
    for (int i = passos - 1; i >= 0; i--) { //preenche o m_caminho com os nodes que o fantasma tem que percorrer ate o node final(pacman)
        m_caminho[i] = atual;
        atual = atual->parent;
    }
}

bool AstarAlgoritmo::calcularCaminho(int linhaInicio, int colunaInicio, int linhaFim, int colunaFim) {
    auto startTempo = std::chrono::high_resolution_clock::now(); //marca inicio da contagem de tempo que a prof pediu
    m_nosGerados = 0;
    m_nosExpandidos = 0;
    m_tamanhoCaminho = 0;
    m_caminho.fill(nullptr);

    bool dentroDosLimites =
        linhaInicio >= 0 && linhaInicio < LINHAS && colunaInicio >= 0 && colunaInicio < COLUNAS &&
        linhaFim >= 0 && linhaFim < LINHAS && colunaFim >= 0 && colunaFim < COLUNAS;

    if (!dentroDosLimites) {
        printf("ta entrando aqui fora dos limites\n");
        return false;
    }
    if (m_grid[linhaInicio][colunaInicio].is_obstacle || m_grid[linhaFim][colunaFim].is_obstacle) { //olha no grid se os fantasmas ou o pacman estao dentro de um obstaculo
        printf("ta entrando aqui parede\n");
        return false;
    }

    resetarGrid();

    Node* start = &m_grid[linhaInicio][colunaInicio]; //aponta node inical
    Node* end = &m_grid[linhaFim][colunaFim]; //aponta node final

    start->g_cost = {};
    start->h_cost = heuristica(start, end);
    start->f_cost = start->g_cost + start->h_cost;

    m_tamanhoListaAberta = 0; //lista limpa
    m_listaAberta[m_tamanhoListaAberta++] = start; //insere node inicial na lista aberta
    m_nosGerados++; //o node inicial conta como gerado para a metrica de nos gerados

    if (start == end) { //caso o inicio seja o proprio destino
        reconstruirCaminho(start);
        auto endTempo = std::chrono::high_resolution_clock::now();
        m_tempoMs = std::chrono::duration<double, std::milli>(endTempo - startTempo).count(); //conta quanto tempo passou
        return true;
    }

    while (m_tamanhoListaAberta > 0) { //busca linear buscando o node com menor f_cost
        //mais lenta porem mais facil e o grid nao e grande o bastante pra impactar no desempenho

        int indiceMenorF{};
        for (int i = 1; i < m_tamanhoListaAberta; i++) {
            if (m_listaAberta[i]->f_cost < m_listaAberta[indiceMenorF]->f_cost) { //substitui se o valor de f atual for o menor encontrado ate agora
                indiceMenorF = i;
            }
        }

        Node* atual = m_listaAberta[indiceMenorF]; //aponta pro melhor node da lista

        m_listaAberta[indiceMenorF] = m_listaAberta[m_tamanhoListaAberta - 1]; //remove o atual da lista aberta, troca com o ultimo elemento
        m_tamanhoListaAberta--;

        atual->visitado = true;
        m_nosExpandidos++; //todo node retirado da lista conta pros expandidos

        if (atual == end) { //quando chega ao destino chama o reconstruirCaminho e encerra
            reconstruirCaminho(atual);
            auto endTempo = std::chrono::high_resolution_clock::now();
            m_tempoMs = std::chrono::duration<double, std::milli>(endTempo - startTempo).count();
            return true;
        }

        const int moveLinha[4]{ -1, 1, 0, 0 }; //direcoes pra mover linha, cime e baixo
        const int moveColuna[4]{ 0, 0, -1, 1 }; //direcoes pra mover coluna, esquerda e direita

        for (int dir{}; dir < 4; dir++) { //dir e direcao
            int novaLinha = atual->linha + moveLinha[dir];
            int novaColuna = atual->coluna + moveColuna[dir];

            if (novaLinha < 0 || novaLinha >= LINHAS || novaColuna < 0 || novaColuna >= COLUNAS) { //ignora fora do mapa
                continue;
            }

            Node* vizinho = &m_grid[novaLinha][novaColuna]; //pega o node vizinho na direcao atual

            if (vizinho->is_obstacle || vizinho->visitado) { //ignora obstaculo ou ja visitado
                continue;
            }

            float novoG = atual->g_cost + 1.0f; //o g_cost sempre aumenta de 1 em 1 ja que o mapa e uniforme

            if (novoG < vizinho->g_cost) { //so atualiza o vizinho se o caminho ate ele for melhor
                bool jaEstaNaListaAberta = (vizinho->g_cost != std::numeric_limits<float>::max()); //checa se o node ja foi descoberto antes

                vizinho->g_cost = novoG;
                vizinho->h_cost = heuristica(vizinho, end);
                vizinho->f_cost = vizinho->g_cost + vizinho->h_cost;
                vizinho->parent = atual; //marca por qual node chegou ao vizinho pra reconstruir depois

                if (!jaEstaNaListaAberta) { //insere na lista
                    m_listaAberta[m_tamanhoListaAberta++] = vizinho;
                    m_nosGerados++; //adiciona mais um pra metrica dos nos gerados
                }
            }
        }
    }

    auto endTempo = std::chrono::high_resolution_clock::now();
    m_tempoMs = std::chrono::duration<double, std::milli>(endTempo - startTempo).count(); //conta quanto tempo passou
    return false; //se lista aberta esgotar sem alcancar o destino e porque nao tem caminho
}

std::vector<std::pair<int, int>> AstarAlgoritmo::getMCaminhoAsVector() {
    std::vector<std::pair<int, int>> route;

    for (auto i : m_caminho) {
        if (i == nullptr) { continue; }

        int y = i->linha;
        int x = i->coluna;

        route.emplace_back(std::make_pair(x, y));
    }

    return route;
}
