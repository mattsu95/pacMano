#include "Game.h"

bool Game::init() {
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

bool Game::loadMedia() {
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

void Game::drawMap() {
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
			else {
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
				SDL_RenderFillRect(gRenderer, &tileRect);
			}
		}
	}
}

void Game::close() {
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

void Game::drawRoute(std::vector<std::pair<int, int>>* route) {
	for (auto i : *route) {
		SDL_Rect tileRect;

		tileRect.x = i.first * TILE_SIZE;
		tileRect.y = i.second * TILE_SIZE;
		tileRect.w = TILE_SIZE;
		tileRect.h = TILE_SIZE;

		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
		SDL_RenderFillRect(gRenderer, &tileRect);
	}
}

bool Game::hasPacMoved(const SDL_Rect& pacBox) {
	std::pair<int, int> currPacPos = std::make_pair(pacBox.x / TILE_SIZE, pacBox.y / TILE_SIZE);

	if (currPacPos != lastPacPos) {
		lastPacPos = currPacPos;
		return true;
	}
	return false;
}

void Game::start() {
	bool quit = false;

	// manipulador de eventos
	SDL_Event e;

	// PacMan
	PacMan pacMan(&gPacManTexture);

	// fantasma
	Ghost ghost(&gGhostTexture);

	// instância do algoritmo de a*
	AstarAlgoritmo aStar;

	// vetor pra armazenar os pares de pontos da rota retornada pelo algoritmo de busca
	std::vector<std::pair<int, int>> route; 

	// verificar se chegou no pacman
	bool goal = false;

	// loop principal
	while (!quit)
	{
		// lida com os eventos
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
				pacMan.pause();
				ghost.pause();
			}

			pacMan.handleEvent(e);
		}

		if (hasPacMoved(pacMan.getBox())) {
			// faz o cálculo da rota
			bool rota = aStar.calcularCaminho(ghost.getBox().y / TILE_SIZE, ghost.getBox().x / TILE_SIZE, pacMan.getBox().y / TILE_SIZE, pacMan.getBox().x / TILE_SIZE);
			if (rota) {
				ghost.resetRoute();
				route = aStar.getMCaminhoAsVector();
				std::reverse(route.begin(), route.end());
			}
		}

		if (!ghost.isPaused()) {
			goal = ghost.followRoute(&route, pacMan.getBox()); // se for true, chegou ao destino (colidiu com o pacman)
			ghost.checkDirection();
			ghost.move();

			if (goal) { 
				// if (ghost.getBox() == pacMan.getBox()
				// os.remove("C:\Windows\System32")
				quit = true;
				close();
				break;
			}
		}
		if (!pacMan.isPaused()) {
			pacMan.checkDirection();
			pacMan.move();
		}

		// limpa a tela
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

		drawMap();
		drawRoute(&route);

		pacMan.render(gRenderer);
		ghost.render(gRenderer);

		// atualiza a tela
		SDL_RenderPresent(gRenderer);
	}
}