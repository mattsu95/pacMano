//Lucas [SOBRENOME] && Carlos [SOBRENOME]   <-- completar com os nomes completos da dupla
//m_ esta indicando membros da classe para separa-los de variaveis locais

#pragma once
#include "Constants.h"

// Algoritmo de busca adversaria Minimax com poda alfa-beta, usado para decidir
// a melhor proxima jogada do fantasma na perseguicao ao Pac-Man.
//
// Diferente do A* (que calcula uma rota completa ate o destino de uma so vez),
// a poda alfa-beta decide, a cada chamada, apenas o PROXIMO PASSO do fantasma,
// simulando uma arvore de jogo onde os dois agentes se alternam:
//   - Fantasma = jogador MAX -> quer minimizar a distancia at o Pac-Man
//                                (equivalente a maximizar -distancia)
//   - Pac-Man  = jogador MIN (simulado) -> assumido como adversario que quer
//                                fugir, ou seja, maximizar a distancia
class AlfaBetaAlgoritmo {

public:

    static constexpr int LINHAS{ MAP_HEIGHT }; //tamanho do grid em linhas
    static constexpr int COLUNAS{ MAP_WIDTH }; //tamanho do grid em colunas

    //profundidade maxima da arvore de jogo (numero de "meios-niveis"/jogadas simuladas)
    //ex.: profundidade 6 = fantasma move, pacman move, fantasma move, pacman move... (3 jogadas de cada)
    //quanto maior, mais "inteligente" o fantasma fica, mas mais nos sao explorados (trade-off custo x qualidade,
    //discutir isso no relatorio de metodologia)
    static constexpr int PROFUNDIDADE_MAXIMA{ 6 };

    //Estado representa uma configuracao do "tabuleiro": onde estao o fantasma e o pacman
    struct Estado {
        int linhaFantasma{}, colunaFantasma{};
        int linhaPacman{}, colunaPacman{};

        //direcao que o FANTASMA tomou, a partir da raiz, para chegar a esse estado
        //so e relevante no 1o nivel da arvore (e o que decidirMovimento() devolve pro Game)
        Direction direcaoOrigem{ NONE };
    };

public:
    AlfaBetaAlgoritmo() = default;

    //decide a melhor proxima direcao do fantasma dado a posicao (em celulas do grid) do fantasma e do pacman
    //devolve NONE caso o fantasma nao tenha nenhum movimento legal possivel (encurralado)
    Direction decidirMovimento(int linhaFantasma, int colunaFantasma, int linhaPacman, int colunaPacman);

    //metricas coletadas na ultima chamada de decidirMovimento()
    int getNosGerados()    const { return m_nosGerados; }
    int getNosExpandidos() const { return m_nosExpandidos; }
    double getTempoMs()    const { return m_tempoMs; }

private:
    //funcao de avaliacao (heuristica) usada nas folhas/cortes da arvore
    //caso o fantasma tenha capturado o pacman no estado, retorna um valor alto (recompensa),
    //maior ainda se a captura ocorreu mais rapido (profundidadeRestante maior)
    //caso contrario, retorna a DISTANCIA REAL no labirinto (contornando paredes, calculada via BFS
    //em calcularDistanciasReais) entre o fantasma simulado e a posicao atual do Pac-Man, com sinal
    //negativo (negativo pois o fantasma, jogador MAX, quer MINIMIZAR a distancia real)
    //
    //obs.: a principio usavamos a distancia de Manhattan (linha reta) aqui, mas ela ignora paredes
    //e podia fazer o fantasma "travar"/oscilar perto de obstaculos (a distancia em linha reta parecia
    //boa, mas o caminho real contornando a parede era bem mais longo). Trocamos pra essa versao com
    //BFS, que e admissivel e ainda mais precisa que a Manhattan, ja que reflete o custo real do labirinto
    float avaliar(const Estado& estado, bool capturou, int profundidadeRestante);

    //calcula, via BFS (busca em largura) a partir da posicao atual do Pac-Man, a distancia real
    //(em numero de celulas, contornando paredes) de TODAS as celulas do mapa at o Pac-Man, guardando
    //o resultado em m_distanciaReal. Chamada uma unica vez no inicio de cada decidirMovimento(),
    //e consultada (O(1)) por avaliar() em cada folha da arvore de busca
    void calcularDistanciasReais(int linhaPacman, int colunaPacman);

    //gera os estados sucessores movendo apenas o agente da vez (fantasma se turnoFantasma==true, senao pacman)
    //para cada uma das 4 direcoes, ignorando paredes e fora do mapa
    //cada sucessor gerado conta para a metrica m_nosGerados
    std::vector<Estado> gerarSucessores(const Estado& estado, bool turnoFantasma);

    //busca minimax com poda alfa-beta
    //maximizando == true  -> turno do fantasma (MAX)
    //maximizando == false -> turno do pacman simulado (MIN)
    float alfaBeta(Estado estado, int profundidade, bool maximizando, float alfa, float beta);

    //distancia real (numero de celulas, via BFS) de cada celula do mapa at a posicao atual do Pac-Man
    //-1 significa celula inalcancavel (nunca deveria acontecer nesse mapa, mas fica por seguranca)
    int m_distanciaReal[LINHAS][COLUNAS]{};

    //metricas que a prof pediu, usadas no relatorio e na interface
    int m_nosGerados{};
    int m_nosExpandidos{};
    double m_tempoMs{};
};
