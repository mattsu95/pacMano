#include <cmath>
#include "PacMan.h"
#include "Ghost.h"

// janela principal
SDL_Window* gWindow = NULL;

// renderer da janela
SDL_Renderer* gRenderer = NULL;

// texturas da cena
LTexture gPacManTexture;
LTexture gGhostTexture;

bool init() {
	bool success = true;

	// inicializa o SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Nao foi possivel incializar o SDL! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// seta o filtro da textura linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("AVISO: Filtro de textura linear inativo!");
		}

// ====================== mudar o nome da janela quando for fazer pro outro algoritmo. Pensamos em como depois ======================
		gWindow = SDL_CreateWindow("Pac Man A* Edition", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Nao foi possivel criar a janela! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				printf("Nao foi possivel criar o Renderer! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				// inicializa a cor do renderer
				SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

				// inicializa o carregamento de PNG
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("Nao foi possivel inicializar o SDL_image! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia() {
	bool success = true;

	// textura do Pac
	if (!gPacManTexture.loadFromFile("assets/PacMan2.bmp", gRenderer)) {
		printf("Falha no carregamento da textura do Pac!\n");
		success = false;
	}

	// textura do fantasma
	if (!gGhostTexture.loadFromFile("assets/Ghost.bmp", gRenderer)) {
		printf("Falha no carregamento da textura do Ghost!");
		success = false;
	}

	return success;
}

void drawMap() {
	for (int i = 0; i < MAP_HEIGHT; ++i) {
		for (int j = 0; j < MAP_WIDTH; ++j) {
			SDL_Rect tileRect;

			tileRect.x = j * TILE_SIZE;
			tileRect.y = i * TILE_SIZE;
			tileRect.w = TILE_SIZE;
			tileRect.h = TILE_SIZE;

			if (mapa[i][j] == 1) {
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
				SDL_RenderFillRect(gRenderer, &tileRect);
			}


		}
	}
}

void close() {
	gPacManTexture.free();
	gGhostTexture.free();

    // destroi a janela  
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    // fecha os subsistemas SDL
    IMG_Quit();
    SDL_Quit();
}

void start() {
	bool quit = false;

	// manipulador de eventos
	SDL_Event e;

	// PacMan
	PacMan pacMan(&gPacManTexture);

	// fantasma
	Ghost ghost(&gGhostTexture);

	std::vector<std::pair<int, int>> route;

	route.emplace_back(14, 17);
	route.emplace_back(14, 18);
	route.emplace_back(14, 19);
	route.emplace_back(13, 19);

	std::reverse(route.begin(), route.end());

	// loop principal
	while (!quit)
	{
		// lida com os eventos
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			pacMan.handleEvent(e);
		}

		ghost.checkDirection(&route);
		ghost.move();
		pacMan.checkDirection();		
		pacMan.move();

		// limpa a tela
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);


		pacMan.render(gRenderer);
		ghost.render(gRenderer);

		drawMap();

		// atualiza a tela
		SDL_RenderPresent(gRenderer);
	}
}

int main(int argc, char* args[]) {
	// inicia SDL e cria janela
	if (!init()) {
		printf("Falha na inicializacao!\n");
	}
	else {
		// carrega midia utilizada
		if (!loadMedia()) {
			printf("Falha ao carregar midia!\n");
		}
		else {
			start();
		}
	}

	close();

	return 0;
}
