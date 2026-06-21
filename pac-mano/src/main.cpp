#include <cmath>
#include "LTexture.h"
#include "PacMan.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// scene textures
LTexture gPacManTexture;



bool init()
{
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		// set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		// mudar o nome da janela quando for fazer pro outro algoritmo. Pensamos em como depois
		gWindow = SDL_CreateWindow("Pac Man A* Edition", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				// initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				// initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG surface
	/*gTexture = IMG_LoadTexture(gRenderer, "assets/texture.png");
	if (gTexture == NULL)
	{
		printf("Failed to load texture image!\n");
		success = false;
	}*/

	// load PacMan texture
	if (!gPacManTexture.loadFromFile("assets/PacMan.bmp", gRenderer)) {
		printf("Failed to load texture image!\n");
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

void close()
{
	//Free loaded image
    // SDL_DestroyTexture(gTexture);
    //gTexture = NULL;
	gPacManTexture.free();

    //Destroy window    
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void mainLoop() {
	bool quit = false;

	// Event handler
	SDL_Event e;

	// PacMan
	PacMan pacMan(&gPacManTexture);

	// enquanto a aplicação tá rodando
	while (!quit)
	{
		// lida com eventos na fila
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			pacMan.handleEvent(e);
		}

		pacMan.checkDirection();
		pacMan.move();

		// limpa a tela
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);


		pacMan.render(gRenderer);

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
			printf("falha ao carregar midia!\n");
		}
		else {
			mainLoop();
		}
	}

	close();

	return 0;
}
