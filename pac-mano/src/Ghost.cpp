#include "Ghost.h"

Ghost::Ghost(LTexture* texture): Entity(texture) {
	mBox.x = 13 * TILE_SIZE;
	mBox.y = 17 * TILE_SIZE;
	actDirection = RIGHT;
	nxtDirection = RIGHT;
	nxtPos = { 13, 17 };
}

void Ghost::checkDirection(std::vector<std::pair<int, int>>* route) {
	int actX = mBox.x / TILE_SIZE;
	int actY = mBox.y / TILE_SIZE;
	if (std::make_pair(actX, actY) != nxtPos) { return; }

	if (route == nullptr || route->empty()) { return; }
	auto nxt = route->back();
	nxtPos = nxt;
	route->pop_back();

	std::pair<int, int> direita = { actX + 1, actY };
	std::pair<int, int> esquerda = { actX - 1, actY };
	std::pair<int, int> baixo = { actX, actY + 1 };
	std::pair<int, int> cima = { actX, actY - 1 };

	printf("%d %d \n", nxt.first, nxt.second);
	


	if (nxt == direita) {
		actDirection = RIGHT;
	}
	if (nxt == esquerda) {
		actDirection = LEFT;
	}
	if (nxt == baixo) {
		actDirection = DOWN;
	}
	if (nxt == cima) {
		actDirection = UP;
	}

	setVel();
}

void Ghost::move() {
	mBox.x += mVelX;

	// testa colisão considerando todos os cantos da entidade
	int lSup = mBox.y / TILE_SIZE; // linha superior
	int lInf = (mBox.y + mBox.h - 1) / TILE_SIZE; // linha inferior
	int cEsq = mBox.x / TILE_SIZE; // coluna esquerda
	int cDir = (mBox.x + mBox.w - 1) / TILE_SIZE; // coluna direita

	// verifica se existe uma parede em cada canto
	bool collision = (mapa[lSup][cEsq] == 1 || mapa[lInf][cEsq] == 1 || mapa[lSup][cDir] == 1 || mapa[lInf][cDir] == 1);

	if (cEsq < 0 || cDir > MAP_WIDTH || collision) {
		// move back
		mBox.x -= mVelX;
	}
	mBox.y += mVelY;

	lSup = mBox.y / TILE_SIZE; // linha superior
	lInf = (mBox.y + mBox.h - 1) / TILE_SIZE; // linha inferior
	cEsq = mBox.x / TILE_SIZE; // coluna esquerda
	cDir = (mBox.x + mBox.w - 1) / TILE_SIZE; // coluna direita

	// verifica se existe uma parede em cada canto
	collision = (mapa[lSup][cEsq] == 1 || mapa[lInf][cEsq] == 1 || mapa[lSup][cDir] == 1 || mapa[lInf][cDir] == 1);

	if (lSup < 0 || lInf > MAP_HEIGHT || collision) {
		// move back
		mBox.y -= mVelY;
	}
}