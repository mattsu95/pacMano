#include "Ghost.h"

Ghost::Ghost(LTexture* texture): Entity(texture) {
	mBox.x = 14 * TILE_SIZE;
	mBox.y = 17 * TILE_SIZE;
	nxtPos = { 14, 17 };
	prevPos = nxtPos;
}

void Ghost::resetRoute() {
	nxtPos = { mBox.x / TILE_SIZE, mBox.y / TILE_SIZE };
}

bool Ghost::followRoute(std::vector<std::pair<int, int>>* route, SDL_Rect pacBox) {
	int actX = mBox.x / TILE_SIZE;
	int actY = mBox.y / TILE_SIZE;
	int pacX = pacBox.x / TILE_SIZE;
	int pacY = pacBox.y / TILE_SIZE;

	if (actX == pacX && actY == pacY) { return true; }

	if (route == nullptr || route->empty()) { // se chegar ao final da rota (pacMan)
		return false;
	}

	if (std::make_pair(actX, actY) != nxtPos) { return false; } // pra evitar mudar de direção antes da hora em virtude do arredondamento

	auto nxt = route->back();
	nxtPos = nxt;
	route->pop_back();

	std::pair<int, int> direita = { actX + 1, actY };
	std::pair<int, int> esquerda = { actX - 1, actY };
	std::pair<int, int> baixo = { actX, actY + 1 };
	std::pair<int, int> cima = { actX, actY - 1 };

	if (nxt == direita) {
		nxtDirection = RIGHT;
	}
	if (nxt == esquerda) {
		nxtDirection = LEFT;
	}
	if (nxt == baixo) {
		nxtDirection = DOWN;
	}
	if (nxt == cima) {
		nxtDirection = UP;
	}
	
	/*bool rotaRecalculada = aStar->calcularCaminho(mBox.y / TILE_SIZE, mBox.x / TILE_SIZE, pacBox.y / TILE_SIZE, pacBox.x / TILE_SIZE);
	if (rotaRecalculada) {
		*route = aStar->getMCaminhoAsVector();
		std::reverse(route->begin(), route->end());
	}*/

	return false;
}

void Ghost::move() {
	prevPos = { mBox.x, mBox.y };
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

	// gambiarra pra que não fique preso na saída
	const int margem = 15;

	bool naSaida =
		(abs(mBox.x - 14 * TILE_SIZE) <= margem &&
			abs(mBox.y - 15 * TILE_SIZE) <= margem) ||
		(abs(mBox.x - 13 * TILE_SIZE) <= margem &&
			abs(mBox.y - 15 * TILE_SIZE) <= margem);

	if (naSaida && (actDirection == LEFT || actDirection == RIGHT)) {
		mVelX = 0;
		mVelY = -2;
	}
}