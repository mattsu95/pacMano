#include <cmath>
#include "PacMan.h"
#include "Ghost.h"
#include "Game.h"

int main(int argc, char* args[]) {
	// cria instância pro jogo
	Game game;

	// inicia SDL e cria janela
	if (!game.init()) {
		printf("Falha na inicializacao!\n");
	}
	else {
		// carrega midia utilizada
		if (!game.loadMedia()) {
			printf("Falha ao carregar midia!\n");
		}
		else {
			game.start();
		}
	}

	game.close();

	return 0;
}
