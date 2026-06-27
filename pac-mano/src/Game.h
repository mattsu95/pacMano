#pragma once
#include "Constants.h"
#include "LTexture.h"
#include "PacMan.h"
#include "Ghost.h"
#include "Astar.h"	

class Game {
public:
	// construtor (inicializa as entidades)
	Game();

	// janela principal
	SDL_Window* gWindow = NULL;

	// renderer da janela
	SDL_Renderer* gRenderer = NULL;

	// texturas do menu
	LTexture gMenuAstarTexture;
	LTexture gMenuAlphaBetaTexture;

	// texturas da cena
	LTexture gPacManTexture;
	LTexture gGhostTexture;

	// fonte usada
	TTF_Font* gFont = NULL;

	// texturas dos textos
	LTexture gMenuTextTexture;
	LTexture gPausedTextTexture;
	LTexture gInGameTextTexture;

	// Função principal -> executa a aplicação
	void start();
	
	// Loop do menu
	void menu(bool* quit);

	// Inicia o SDL e cria uma janela
	bool init();

	// Carrega imagens e sprites
	bool loadMedia();	

	// Abre a fonte
	bool openFont(const int fontSize);

	// Carrega os textos fixos (não carrega o da métrica porque este depende da execução da busca)
	bool loadTexts();	

	// Desenha o mapa
	void drawMap();

	// Libera mídia e desliga o SDL
	void close();

	// Desenha a rota retornada pelo algoritmo
	void drawRoute(std::vector<std::pair<int, int>>* route);

	// Reinicia as entidades
	void restart();

private:
	// Carrega o texto do menu
	bool loadMenuText();

	// Carrega o texto do jogo pausado (métricas)
	bool loadPausedText(int nosGerados, int nosExpandidos, double tempoMs);

	// Carrega o texto do jogo em execução (movimentação e comandos)
	bool loadInGameText();

	// Detecta se o Pacman mudou de célula no grid
	bool hasPacMoved(const SDL_Rect& pacBox);

	// Armazena a posição anterior do Pacman para detectar mudanças
	std::pair<int, int> lastPacPos = { -1, -1 };

	// Define qual vai ser o algoritmo usado
	bool aStarAlg;

	// Entidades
	PacMan pacMan = nullptr;
	Ghost ghost = nullptr;
};


