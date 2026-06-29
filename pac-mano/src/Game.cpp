// Matheus Seghatti e Arthur Pivotto
#include "Game.h"
#include <fstream>

void Game::registrarMetricas(const std::string& algoritmo, int nosGerados, int nosExpandidos, double tempoMs) {
	// verifica se o arquivo ja existe pra so escrever o cabecalho da primeira vez
	std::ifstream testeExistencia("metricas.csv");
	bool arquivoJaExiste = testeExistencia.good();
	testeExistencia.close();

	// abre em modo "append" pra ir acumulando uma linha por jogada, sem apagar o que ja foi gravado
	std::ofstream arquivo("metricas.csv", std::ios::app);
	if (!arquivo.is_open()) {
		printf("Nao foi possivel abrir metricas.csv pra escrita!\n");
		return;
	}

	if (!arquivoJaExiste) {
		arquivo << "algoritmo,nos_gerados,nos_expandidos,tempo_ms\n";
	}

	arquivo << algoritmo << "," << nosGerados << "," << nosExpandidos << "," << tempoMs << "\n";
	arquivo.close();
}


// === Métodos principais ===
void Game::start() {
	bool quit = false;

	// manipulador de eventos
	SDL_Event e;

	// instância do algoritmo de a*
	AstarAlgoritmo aStar;

	// instância do algoritmo de poda alfa-beta
	AlfaBetaAlgoritmo alfaBeta;

	// vetor pra armazenar os pares de pontos da rota retornada pelo algoritmo de busca
	std::vector<std::pair<int, int>> route;

	// verificar se chegou no pacman
	bool goal = false;

	// identificar quando o jogo foi reiniciado
	bool restarted = false;

	// renderiza o menu antes de cair no loop do jogo
	menu(&quit);

	// começa o jogo pausado
	pacMan.pause();
	ghost.pause();
	bool paused = true;

	// métricas
	int nosGerados = 0;
	int nosExpandidos = 0;
	double tempoMs = 0.0;

	// loop principal
	while (!quit) {
		// lida com os eventos
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}

			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_SPACE:
					paused = !paused;
					pacMan.pause();
					ghost.pause();
					break;
				}
			}

			pacMan.handleEvent(e);
		}

		// recalcula a rota se o pac se moveu ou se aconteceu um restart
		if (aStarAlg) {
			// se A* foi selecionado, calcula a rota pelo algoritmo A*
			if (hasPacMoved(pacMan.getBox()) || restarted) {
				// faz o cálculo da rota
				bool rota = aStar.calcularCaminho(ghost.getBox().y / TILE_SIZE, ghost.getBox().x / TILE_SIZE, pacMan.getBox().y / TILE_SIZE, pacMan.getBox().x / TILE_SIZE);

				// constrói o vetor da rota, pega as métricas e carrega a textura do texto
				if (rota) {
					ghost.resetRoute();
					route = aStar.getMCaminhoAsVector();
					std::reverse(route.begin(), route.end());

					nosGerados = aStar.getNosGerados();
					nosExpandidos = aStar.getNosExpandidos();
					tempoMs = aStar.getTempoMs();
					if (!loadPausedText(nosGerados, nosExpandidos, tempoMs)) {
						printf("Erro carregando a textura do texto das metricas!\n");
					}

					// registrarMetricas("A*", nosGerados, nosExpandidos, tempoMs);
				}
				if (restarted) { restarted = false; }
			}
		}
		else {
			// poda alfa-beta: decide so o proximo passo do fantasma, por isso recalcula
			// quando o PROPRIO FANTASMA muda de celula (nao quando o pacman se move, como no A*)
			if (hasGhostMoved(ghost.getBox()) || restarted) {
				Direction direcao = alfaBeta.decidirMovimento(
					ghost.getBox().y / TILE_SIZE, ghost.getBox().x / TILE_SIZE,
					pacMan.getBox().y / TILE_SIZE, pacMan.getBox().x / TILE_SIZE);

				if (direcao != NONE) {
					ghost.setNextDirection(direcao);
				}

				nosGerados = alfaBeta.getNosGerados();
				nosExpandidos = alfaBeta.getNosExpandidos();
				tempoMs = alfaBeta.getTempoMs();
				if (!loadPausedText(nosGerados, nosExpandidos, tempoMs)) {
					printf("Erro carregando a textura do texto das metricas!\n");
				}

				// registrarMetricas("AlfaBeta", nosGerados, nosExpandidos, tempoMs);

				if (restarted) { restarted = false; }
			}
		}

		// aplica as funções de movimento no fantasma e verifica fim de jogo se ele não estiver pausado
		if (!ghost.isPaused()) {
			goal = ghost.followRoute(&route, pacMan.getBox()); // se for true, chegou ao destino (colidiu com o pacman)
			ghost.checkDirection();
			ghost.move();

			// se alcançou o pac, retorna ao menu
			if (goal) {
				// os.remove("C:\Windows\System32")
				menu(&quit);	// volta pro menu
				restart();		// reinicializa as entidades
				route.clear();	// limpa a rota

				// reinicia com as entidades pausadas
				pacMan.pause();
				ghost.pause();
				paused = true;

				// sinaliza restart
				restarted = true;
			}
		}

		// aplica as funções de movimento do pac se ele não estiver pausado
		if (!pacMan.isPaused()) {
			pacMan.checkDirection();
			pacMan.move();
		}

		// limpa a tela
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
		SDL_RenderClear(gRenderer);

		// desenha o mapa e a rota do fantasma
		drawMap();
		drawRoute(&route);

		// renderiza as entidades
		pacMan.render(gRenderer);
		ghost.render(gRenderer);

		// se o jogo tá pausado, renderiza o texto das métricas
		if (paused) {
			gPausedTextTexture.render((SCREEN_WIDTH - gPausedTextTexture.getWidth()) / 2,
				(gPausedTextTexture.getHeight()), gRenderer);
		}
		// renderiza o texto dos comandos
		gInGameTextTexture.render((SCREEN_WIDTH - gInGameTextTexture.getWidth()) / 2,
			(SCREEN_HEIGHT - gInGameTextTexture.getHeight()) - 12, gRenderer);


		// atualiza a tela
		SDL_RenderPresent(gRenderer);
	}
}

void Game::menu(bool* quit) {
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

		// renderiza o texto
		gMenuTextTexture.render((SCREEN_WIDTH - gMenuTextTexture.getWidth()) / 2,
			(SCREEN_HEIGHT - gMenuTextTexture.getHeight()) * 0.80, gRenderer);

		// atualiza a tela
		SDL_RenderPresent(gRenderer);

		if (start) {
			break;
		}
	}
}

// === Construtor ===
Game::Game() {
	PacMan p(&gPacManTexture);
	pacMan = p;

	Ghost g(&gGhostTexture);
	ghost = g;
	aStarAlg = true;
}

// === Métodos auxiliares ===
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

				//Inicializa o SDL_ttf
				if (TTF_Init() == -1) {
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
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

bool Game::openFont(const int fontSize) {
	gFont = TTF_OpenFont("assets/Terminal.ttf", fontSize);
	if (gFont == NULL) {
		printf("Falha no carregamento da fonte do menu! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	return true;
}

bool Game::loadTexts() {
	bool success = true;

	success = loadMenuText();
	success = success ? loadInGameText() : false; // se success ainda for true, recebe o resultado da função
	// se for false, continua sendo false independente do resultado da função

	return success;
}

bool Game::loadMenuText() {
	bool success = true;

	// abre a fonte
	if (!openFont(28)) { success = false; }
	else {
		// renderiza o texto
		std::string texto = "Pressione [Enter]";
		if (!gMenuTextTexture.loadFromRenderedText(texto, gRenderer, gFont, ALIGN::CENTER)) {
			printf("Falha na renderizacao da textura do texto do menu!\n");
			success = false;
		}
	}
	TTF_CloseFont(gFont);
	gFont = NULL;
	return success;
}

bool Game::loadPausedText(int nosGerados, int nosExpandidos, double tempoMs) {
	bool success = true;
	// abre a fonte
	if (!openFont(12)) { success = false; }
	else {
		// renderiza o texto
		std::string texto = "Nos gerados: " + std::to_string(nosGerados) + "; " + "Nos expandidos: " + std::to_string(nosExpandidos) + "\n"
			+ "Tempo de execucao (ms): " + std::to_string(tempoMs);
		if (!gPausedTextTexture.loadFromRenderedText(texto, gRenderer, gFont, ALIGN::LEFT)) {
			printf("Falha na renderizacao da textura do texto do jogo pausado!\n");
			success = false;
		}
	}
	TTF_CloseFont(gFont);
	gFont = NULL;
	return success;
}

bool Game::loadInGameText() {
	bool success = true;

	// abre a fonte
	if (!openFont(15)) { success = false; }
	else {
		// renderiza o texto
		std::string texto = "Use as setinhas para se mover!\nPressione [espaco] para pausar!";
		if (!gInGameTextTexture.loadFromRenderedText(texto, gRenderer, gFont, ALIGN::CENTER)) {
			printf("Falha na renderizacao da textura do texto do jogo em execucao!\n");
			success = false;
		}
	}
	TTF_CloseFont(gFont);
	gFont = NULL;
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
	// libera as texturas
	gMenuAlphaBetaTexture.free();
	gMenuAstarTexture.free();
	gPacManTexture.free();
	gGhostTexture.free();
	gMenuTextTexture.free();
	gPausedTextTexture.free();
	gInGameTextTexture.free();

	// destroi a janela  
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	// fecha os subsistemas SDL
	TTF_Quit();
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

bool Game::hasGhostMoved(const SDL_Rect& ghostBox) {
	// so considera "mudou de celula" quando o fantasma esta EXATAMENTE alinhado ao grid
	// (multiplo exato de TILE_SIZE em x e y). Sem essa checagem, a divisao inteira (x/TILE_SIZE)
	// fazia o "indice da celula" mudar 2 pixels ANTES do alinhamento perfeito, e a poda alfa-beta
	// trocava de direcao no meio de uma celula. Isso deixava a caixa do fantasma desalinhada por
	// alguns pixels, fazendo ela "tocar" em duas linhas/colunas do mapa ao mesmo tempo e colidir
	// com uma parede vizinha que nao devia importar -> o fantasma travava pra sempre nesse ponto
	if (ghostBox.x % TILE_SIZE != 0 || ghostBox.y % TILE_SIZE != 0) {
		return false;
	}

	std::pair<int, int> currGhostPos = std::make_pair(ghostBox.x / TILE_SIZE, ghostBox.y / TILE_SIZE);

	if (currGhostPos != lastGhostPos) {
		lastGhostPos = currGhostPos;
		return true;
	}
	return false;
}

void Game::restart() {
	PacMan p(&gPacManTexture);
	pacMan = p;

	Ghost g(&gGhostTexture);
	ghost = g;

	// reseta o "cache" de ultima posicao conhecida, garantindo que tanto o A* quanto a poda
	// alfa-beta recalculem a jogada ja no primeiro frame da nova partida, em vez de pensar
	// que o pacman/fantasma ainda esta na mesma celula em que estava quando a partida anterior
	// terminou (isso podia fazer o fantasma "esquecer" de se mover apos um restart)
	lastPacPos = { -1, -1 };
	lastGhostPos = { -1, -1 };
}

