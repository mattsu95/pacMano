#pragma once
#include "Constants.h"
#include "LTexture.h"
#include "PacMan.h"
#include "Ghost.h"

class Game {
public:
	// janela principal
	SDL_Window* gWindow = NULL;

	// renderer da janela
	SDL_Renderer* gRenderer = NULL;

	// texturas da cena
	LTexture gPacManTexture;
	LTexture gGhostTexture;

	// Métodos
	// Inicia o SDL e cria uma janela
	bool init();

	// Carrega imagens e sprites
	bool loadMedia();	

	// Desenha o mapa
	void drawMap();

	// Roda a aplicação
	void start();

	// Verifica se o fantasma pegou o Pac
	bool checkEndGame(PacMan* p, Ghost* g);

	// Libera mídia e desliga o SDL
	void close();
};


