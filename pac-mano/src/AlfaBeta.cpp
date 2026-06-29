//Lucas [SOBRENOME] && Carlos [SOBRENOME]   <-- completar com os nomes completos da dupla
//m_ esta indicando membros da classe para separa-los de variaveis locais

#include "AlfaBeta.h"
#include <cmath>
#include <chrono>
#include <limits>
#include <algorithm>
#include <queue>

void AlfaBetaAlgoritmo::calcularDistanciasReais(int linhaPacman, int colunaPacman) {
    //inicializa todas as celulas como "nao visitada ainda"
    for (int l{}; l < LINHAS; l++) {
        for (int c{}; c < COLUNAS; c++) {
            m_distanciaReal[l][c] = -1;
        }
    }

    const int moveLinha[4]{ -1, 1, 0, 0 };
    const int moveColuna[4]{ 0, 0, -1, 1 };

    std::queue<std::pair<int, int>> fila; //fila do BFS (busca em largura)
    m_distanciaReal[linhaPacman][colunaPacman] = 0; //distancia do pacman pra ele mesmo e zero
    fila.push({ linhaPacman, colunaPacman });

    while (!fila.empty()) {
        std::pair<int, int> atual = fila.front();
        fila.pop();

        for (int dir{}; dir < 4; dir++) { //dir e direcao
            int novaLinha = atual.first + moveLinha[dir];
            int novaColuna = atual.second + moveColuna[dir];

            bool dentroDosLimites = novaLinha >= 0 && novaLinha < LINHAS && novaColuna >= 0 && novaColuna < COLUNAS;
            if (!dentroDosLimites) { continue; } //ignora fora do mapa
            if (mapa[novaLinha][novaColuna] == 1) { continue; } //ignora parede
            if (m_distanciaReal[novaLinha][novaColuna] != -1) { continue; } //ja visitado (BFS so visita uma vez)

            //como o BFS expande "camada por camada", a primeira vez que uma celula e alcancada
            //e sempre pelo caminho mais curto (todas as arestas tem o mesmo "peso" 1)
            m_distanciaReal[novaLinha][novaColuna] = m_distanciaReal[atual.first][atual.second] + 1;
            fila.push({ novaLinha, novaColuna });
        }
    }
}

float AlfaBetaAlgoritmo::avaliar(const Estado& estado, bool capturou, int profundidadeRestante) {
    if (capturou) {
        //recompensa alta para o fantasma (MAX); soma a profundidade restante pra preferir
        //capturas mais rapidas (que sobram mais niveis) sobre capturas mais demoradas
        return 1000.0f + static_cast<float>(profundidadeRestante);
    }

    //consulta a distancia REAL (contornando paredes) ja calculada por calcularDistanciasReais(),
    //em vez da distancia de Manhattan (que ignora paredes e podia enganar o fantasma perto de obstaculos)
    int distancia = m_distanciaReal[estado.linhaFantasma][estado.colunaFantasma];
    if (distancia < 0) {
        //celula inalcancavel pelo fantasma nesse mapa (nao deveria acontecer, mas evita usar -1 como se fosse otimo)
        distancia = LINHAS * COLUNAS;
    }

    //sinal negativo, pois o fantasma (MAX) quer o MENOR valor de distancia possivel,
    //ou seja, o MAIOR valor de -distancia
    return -static_cast<float>(distancia);
}

std::vector<AlfaBetaAlgoritmo::Estado> AlfaBetaAlgoritmo::gerarSucessores(const Estado& estado, bool turnoFantasma) {
    std::vector<Estado> sucessores;

    const int moveLinha[4]{ -1, 1, 0, 0 };  //direcoes pra mover linha, cima e baixo
    const int moveColuna[4]{ 0, 0, -1, 1 }; //direcoes pra mover coluna, esquerda e direita
    const Direction moveDirecao[4]{ UP, DOWN, LEFT, RIGHT };

    //define qual agente esta se movendo nesse nivel da arvore; o outro permanece parado
    int linhaBase = turnoFantasma ? estado.linhaFantasma : estado.linhaPacman;
    int colunaBase = turnoFantasma ? estado.colunaFantasma : estado.colunaPacman;

    for (int dir{}; dir < 4; dir++) { //dir e direcao
        int novaLinha = linhaBase + moveLinha[dir];
        int novaColuna = colunaBase + moveColuna[dir];

        bool dentroDosLimites = novaLinha >= 0 && novaLinha < LINHAS && novaColuna >= 0 && novaColuna < COLUNAS;
        if (!dentroDosLimites) { continue; } //ignora fora do mapa
        //obs.: nao trata o tunel lateral (linha 17) aqui pra simplificar a arvore de busca;
        //isso pode ser citado como uma simplificacao/limitacao no relatorio

        if (mapa[novaLinha][novaColuna] == 1) { continue; } //ignora parede

        Estado filho = estado; //copia o estado atual (mantem a posicao do agente que nao se moveu)

        if (turnoFantasma) {
            filho.linhaFantasma = novaLinha;
            filho.colunaFantasma = novaColuna;
            filho.direcaoOrigem = moveDirecao[dir]; //relevante apenas no 1o nivel (decidirMovimento)
        }
        else {
            filho.linhaPacman = novaLinha;
            filho.colunaPacman = novaColuna;
            //mantem a direcao do fantasma que originou esse ramo, pra propagar at a raiz
            filho.direcaoOrigem = estado.direcaoOrigem;
        }

        sucessores.push_back(filho);
        m_nosGerados++; //cada sucessor criado conta para a metrica de nos gerados
    }

    return sucessores;
}

float AlfaBetaAlgoritmo::alfaBeta(Estado estado, int profundidade, bool maximizando, float alfa, float beta) {
    bool capturou = (estado.linhaFantasma == estado.linhaPacman && estado.colunaFantasma == estado.colunaPacman);

    if (profundidade == 0 || capturou) { //corte por profundidade ou captura: no terminal/folha
        return avaliar(estado, capturou, profundidade);
    }

    //maximizando indica de quem e a vez: true = fantasma, false = pacman simulado
    std::vector<Estado> filhos = gerarSucessores(estado, maximizando);

    if (filhos.empty()) { //agente da vez ficou encurralado (sem movimento legal): trata como folha
        return avaliar(estado, capturou, profundidade);
    }

    m_nosExpandidos++; //esse no teve seus sucessores gerados, conta pra metrica de nos expandidos

    if (maximizando) { //turno do fantasma (MAX) - quer o maior valor (= menor distancia real)
        float melhor = -std::numeric_limits<float>::infinity();
        for (auto& filho : filhos) {
            float valor = alfaBeta(filho, profundidade - 1, false, alfa, beta);
            melhor = std::max(melhor, valor);
            alfa = std::max(alfa, melhor);
            if (alfa >= beta) { break; } //poda beta: o MIN ja garante algo <= beta em outro ramo, nao vale continuar
        }
        return melhor;
    }
    else { //turno do pacman simulado (MIN) - quer o menor valor (= maior distancia real)
        float pior = std::numeric_limits<float>::infinity();
        for (auto& filho : filhos) {
            float valor = alfaBeta(filho, profundidade - 1, true, alfa, beta);
            pior = std::min(pior, valor);
            beta = std::min(beta, pior);
            if (alfa >= beta) { break; } //poda alfa: o MAX ja garante algo >= alfa em outro ramo, nao vale continuar
        }
        return pior;
    }
}

Direction AlfaBetaAlgoritmo::decidirMovimento(int linhaFantasma, int colunaFantasma, int linhaPacman, int colunaPacman) {
    auto inicioTempo = std::chrono::high_resolution_clock::now(); //marca inicio da contagem de tempo que a prof pediu
    m_nosGerados = 0;
    m_nosExpandidos = 0;

    //calcula, uma unica vez, a distancia real (contornando paredes) de cada celula do mapa
    //ate a posicao ATUAL do pacman; essa tabela e usada pela funcao avaliar() em toda a arvore
    calcularDistanciasReais(linhaPacman, colunaPacman);

    Estado raiz{};
    raiz.linhaFantasma = linhaFantasma;
    raiz.colunaFantasma = colunaFantasma;
    raiz.linhaPacman = linhaPacman;
    raiz.colunaPacman = colunaPacman;
    raiz.direcaoOrigem = NONE;
    m_nosGerados++; //o estado raiz conta como gerado, igual e feito no A*

    float alfa = -std::numeric_limits<float>::infinity();
    float beta = std::numeric_limits<float>::infinity();
    Direction melhorDirecao = NONE;
    float melhorValor = -std::numeric_limits<float>::infinity();

    //1o nivel da arvore: o fantasma escolhe qual das 4 direcoes seguir
    std::vector<Estado> filhosDaRaiz = gerarSucessores(raiz, true);

    if (!filhosDaRaiz.empty()) {
        m_nosExpandidos++; //a raiz foi expandida

        for (auto& filho : filhosDaRaiz) {
            //a partir daqui e a vez do pacman simulado (MIN)
            float valor = alfaBeta(filho, PROFUNDIDADE_MAXIMA - 1, false, alfa, beta);
            if (valor > melhorValor) {
                melhorValor = valor;
                melhorDirecao = filho.direcaoOrigem;
            }
            alfa = std::max(alfa, melhorValor);
            //nao ha poda aqui na raiz: ainda nao existe um jogador MIN acima limitando o valor (beta = +infinito)
        }
    }

    auto fimTempo = std::chrono::high_resolution_clock::now();
    m_tempoMs = std::chrono::duration<double, std::milli>(fimTempo - inicioTempo).count(); //conta quanto tempo passou

    return melhorDirecao; //NONE caso o fantasma esteja encurralado (nenhum movimento legal)
}
