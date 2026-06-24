//Arthur Gomes Pivotto && Matheus Seghatti de Moraes
//m_ esta indicando membros da classe para separá-los de variaveis locais

#pragma once
#include "Game.h"
#include <array>

class AstarAlgoritmo {

public:

    static constexpr int LINHAS{ MAP_HEIGHT }; //tamanho do grid em linhas
    static constexpr int COLUNAS{ MAP_WIDTH }; //tamanho do grid em colunas

    struct Node { //Node representa uma celula do mapa
        int linha{}; //posicao linha do node no grid
        int coluna{}; //posicao coluna do node no grid

        float h_cost{}; //estimativa ate o final
        float g_cost{}; //custo real ate aqui
        float f_cost{}; //custo total estimado F = G + H

        bool is_obstacle{}; //true se for obstaculo
        bool visitado{};    //true se ja foi expandido, vai pra lista fechada

        Node* parent{ nullptr }; //node anterior no caminho pra reconstrucao da rota
    };

public:
    AstarAlgoritmo(); //monta o grid a partir do mapa

    //faz o calculo do caminho da celula (linhaInicio, colunaInicio) ate (linhaFim, colunaFim) e o resultado fica salvo no
    //m_caminho / m_tamanhoCaminho, retorna true se encontrou caminho, false caso nao tenha encontrado
    bool calcularCaminho(int linhaInicio, int colunaInicio, int linhaFim, int colunaFim);

    int getTamanhoCaminho() const { return m_tamanhoCaminho; } //quantos nodes tem o caminho encontrado, se for 0 nao tem caminho

    std::vector <std::pair<int, int>> getMCaminhoAsVector();

    //Devolve a linha/coluna do passo i do caminho encontrado
    //(i = 0 e inicio, i = getTamanhoCaminho()-1 e o destino)
    int getLinhaPasso(int i) const { return m_caminho[i]->linha; } //da a linha do passo i do caminho encontrado
    int getColunaPasso(int i) const { return m_caminho[i]->coluna; } //da a coluna do passo i do caminho encontrado

    //metricas coletadas na ultima chamada de calcularCaminho()
    int getNosGerados()    const { return m_nosGerados; }
    int getNosExpandidos() const { return m_nosExpandidos; }
    double getTempoMs()    const { return m_tempoMs; }

private:
    //heuristica Manhattan: |dx| + |dy|. pode ser usada em grade de 4 direcoes (sem diagonal), nunca superestima o custo real
    float heuristica(const Node* a, const Node* b);

    //percorre os ponteiros parent do node final ate o inicio, e preenchendo m_caminho na ordem correta (inicio ate fim)
    void reconstruirCaminho(Node* nodeFinal);

    //reinicializa g_cost, h_cost, f_cost, visitado e parent de todos os nodes do grid, chamado no comeco de cada calcularCaminho()
    void resetarGrid();


    std::array<std::array<Node, COLUNAS>, LINHAS> m_grid{};

    //aqui e a lista aberta com os nodes candidatos a serem expandidos
    //feita como array simples de ponteiros e busca linear ao inves de lista de prioridade por ser mais facil de entender
    //apesar da lista de prioridade ser mais eficiente o nosso grid nao e grande o bastante pra fazer alguma diferenca real

    static constexpr int MAX_LISTA_ABERTA{ LINHAS * COLUNAS };
    std::array<Node*, MAX_LISTA_ABERTA> m_listaAberta{};
    int m_tamanhoListaAberta{};

    std::array<Node*, MAX_LISTA_ABERTA> m_caminho{}; //resultado do ultimo calculo e a sequencia de nodes do caminho
    int m_tamanhoCaminho{};

    //aqui ficam guardadas as metricas que a prof pediu, lembrar porque precisa pro relatorio e a interface
    int m_nosGerados{};
    int m_nosExpandidos{};
    double m_tempoMs{};
};
