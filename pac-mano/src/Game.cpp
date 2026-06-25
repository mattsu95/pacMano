#include "Game.h"

Game::Game() {
	PacMan p(&gPacManTexture);
	pacMan = p;

	Ghost g(&gGhostTexture);
	ghost = g;
}

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

		gWindow = SDL_CreateWindow("Pac Man (simulation)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

	// texturas do menu
	if (!gMenuAstarTexture.loadFromFile("assets/ngAS.png", gRenderer)) {
		printf("Falha no carregamento da textura 1 do menu!\n");
		success = false;
	}
	if (!gMenuAlphaBetaTexture.loadFromFile("assets/ngABP.png", gRenderer)) {
		printf("Falha no carregamento da textura 2 do menu!\n");
		success = false;
	}

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
	gMenuAlphaBetaTexture.free();
	gMenuAstarTexture.free();
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

void Game::menu(bool *quit) {
	bool start = false;		// flag pra indicar saída do menu e início do jogo
	this->aStarAlg = true;  // define se vai usar a* ou poda alfa-beta

	// define os tamanhos das imagens para que preencham toda a tela
	gMenuAlphaBetaTexture.setWidth(SCREEN_WIDTH);
	gMenuAlphaBetaTexture.setHeight(SCREEN_HEIGHT);

	gMenuAstarTexture.setWidth(SCREEN_WIDTH);
	gMenuAstarTexture.setHeight(SCREEN_HEIGHT);

	SDL_Event e;

	while (!*quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				*quit = true;
			}
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
				switch (e.key.keysym.sym) {
				case SDLK_DOWN:
				case SDLK_UP:
					this->aStarAlg = !this->aStarAlg;
					break;
				case SDLK_ESCAPE:
					*quit = true;
					break;
				case SDLK_RETURN:
					start = true;
					*quit = false;
					break;
				}
			}
		}

		// limpa a tela
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

		// renderiza a imagem atual do menu
		if (this->aStarAlg) {
			gMenuAstarTexture.render(0, 0, gRenderer);
		}
		else {
			gMenuAlphaBetaTexture.render(0, 0, gRenderer);
		}

		// atualiza a tela
		SDL_RenderPresent(gRenderer);

		if (start) {
			break;			
		}
	}
}

void Game::restart() {
	PacMan p(&gPacManTexture);
	pacMan = p;

	Ghost g(&gGhostTexture);
	ghost = g;
}

void Game::start() {
	bool quit = false;

	// manipulador de eventos
	SDL_Event e;

	// instância do algoritmo de a*
	AstarAlgoritmo aStar;

	// vetor pra armazenar os pares de pontos da rota retornada pelo algoritmo de busca
	std::vector<std::pair<int, int>> route; 

	// verificar se chegou no pacman
	bool goal = false;

	// renderiza o menu antes de cair no loop do jogo
	menu(&quit);

	// loop principal
	while (!quit) {
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

		if (hasPacMoved(pacMan.getBox()) || goal) { // recalcula a rota se o pac se moveu ou se aconteceu um restart
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
				// os.remove("C:\Windows\System32")
				menu(&quit);	// volta pro menu
				restart();		// reinicializa as entidades
				route.clear();	// limpa a rota
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