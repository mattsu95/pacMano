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

	// Inicia o SDL e cria uma janela
	bool init();

	// Carrega imagens e sprites
	bool loadMedia();	

	// Desenha o mapa
	void drawMap();

	// Roda a aplicação
	void start();

	// Libera mídia e desliga o SDL
	void close();

	// Desenha a rota retornada pelo algoritmo
	void drawRoute(std::vector<std::pair<int, int>>* route);

	// Loop do menu
	void menu(bool *quit);

	// Reinicia as entidades
	void restart();

private:
	// Armazena a posição anterior do Pacman para detectar mudanças
	std::pair<int, int> lastPacPos = { -1, -1 };

	// Detecta se o Pacman mudou de célula no grid
	bool hasPacMoved(const SDL_Rect& pacBox);

	// Define qual vai ser o algoritmo usado
	bool aStarAlg;

	// Entidades
	PacMan pacMan = nullptr;
	Ghost ghost = nullptr;
};


