// Matheus Seghatti, Arthur Pivotto, Carlos Eduardo e Lucas Henrique

#pragma once
#include "Constants.h"
#include <vector>

// Algoritmo de busca  Minimax com poda alfa-beta, usado para decidir
// a melhor proxima jogada do fantasma na perseguicao ao Pac-Man.
// Fantasma = jogador MAX -> quer minimizar a distancia at o Pac-Man
// Pac-Man  = jogador MIN (simulado) -> assumido como adversario que quer
class AlfaBetaAlgoritmo {

public:

    static constexpr int LINHAS{ MAP_HEIGHT }; //tamanho do grid em linhas
    static constexpr int COLUNAS{ MAP_WIDTH }; //tamanho do grid em colunas

    //profundidade maxima da arvore de jogo (numero de "meios-niveis"/jogadas simuladas)
    //profundidade 6 = fantasma move, pacman move, fantasma move, pacman move... (3 jogadas de cada)
    //quanto maior, mais "inteligente" o fantasma fica, mas mais nos sao explorados
    static constexpr int PROFUNDIDADE_MAXIMA{ 6 };

    //Estado representa uma configuracao do "tabuleiro",onde estao o fantasma e o pacman
    struct Estado {
        int linhaFantasma{}, colunaFantasma{};
        int linhaPacman{}, colunaPacman{};

        //direcao que o FANTASMA tomou, a partir da raiz, para chegar a esse estado
        //so e relevante no 1o nivel da arvore (e o que decidirMovimento() devolve pro Game)
        Direction direcaoOrigem{ NONE };
    };

public:
    AlfaBetaAlgoritmo() = default;

    //decide a melhor proxima direcao do fantasma dado a posicao do fantasma e do pacman
    //devolve NONE caso o fantasma nao tenha nenhum movimento legal possivel (encurralado)
    Direction decidirMovimento(int linhaFantasma, int colunaFantasma, int linhaPacman, int colunaPacman);

    //metricas coletadas na ultima chamada de decidirMovimento()
    int getNosGerados()    const { return m_nosGerados; }
    int getNosExpandidos() const { return m_nosExpandidos; }
    double getTempoMs()    const { return m_tempoMs; }

    //"custo" da jogada atual: distancia real (numero de celulas, contornando paredes) entre o
    //fantasma e o Pac-Man no momento da decisao
    int getCustoAtual() const { return m_custoAtual; }

    //reconstroi a partir da tabela de distancias reais (BFS) calculada na ULTIMA chamada de
    //decidirMovimento(), o caminho mais curto conhecido do fantasma ate o Pac-Man
    std::vector<std::pair<int, int>> getRotaPrevista(int linhaFantasma, int colunaFantasma);

private:
    //funcao de avaliacao (heuristica) usada nas folhas/cortes da arvore
    //caso o fantasma tenha capturado o pacman no estado, retorna um valor alto (recompensa),
    //obs: a principio usavamos a distancia de Manhattan (linha reta) aqui, mas ela ignora paredes
    //e podia fazer o fantasma "travar"perto de obstaculos. Trocamos pra essa versao com
    //BFS, que e admissivel e ainda mais precisa que a Manhattan, ja que reflete o custo real do labirinto
    float avaliar(const Estado& estado, bool capturou, int profundidadeRestante);

    //calcula, via BFS (busca em largura) a partir da posicao atual do Pac-Man
    void calcularDistanciasReais(int linhaPacman, int colunaPacman);

    //gera os estados sucessores movendo apenas o agente da vez 
    std::vector<Estado> gerarSucessores(const Estado& estado, bool turnoFantasma);

    //busca minimax com poda alfa-beta
    float alfaBeta(Estado estado, int profundidade, bool maximizando, float alfa, float beta);

    //distancia real (numero de celulas, via BFS) de cada celula do mapa at a posicao atual do Pac-Man
    //-1 significa celula inalcancavel
    int m_distanciaReal[LINHAS][COLUNAS]{};

    //metricas que a prof pediu, usadas no relatorio e na interface
    int m_nosGerados{};
    int m_nosExpandidos{};
    double m_tempoMs{};
    int m_custoAtual{}; //distancia real (BFS) do fantasma at o pacman na ultima decisao tomada
};
