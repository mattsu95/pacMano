#include "PacMan.h"

PacMan::PacMan(LTexture* texture) : Entity(texture){
	mBox.x = 14 * TILE_SIZE;
	mBox.y = 26 * TILE_SIZE;
	actDirection = RIGHT;
	nxtDirection = RIGHT;
}

void PacMan::handleEvent(SDL_Event& e) {
	if (paused) { return; } // evita mudança de direção se estiver pausado

	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_UP: 
			nxtDirection = UP;
			break;
		case SDLK_DOWN: 
			nxtDirection = DOWN;
			break;
		case SDLK_LEFT:
			nxtDirection = LEFT;
			break;
		case SDLK_RIGHT: 
			nxtDirection = RIGHT;
			break;
		}
	}
}

void PacMan::checkDirection() {
	int nxtX, nxtY;

	switch (nxtDirection) {
	case UP:
		nxtX = mBox.x;
		nxtY = mBox.y - ENTITY_VEL;
		break;
	case DOWN:
		nxtX = mBox.x;
		nxtY = mBox.y + ENTITY_VEL;
		break;
	case LEFT:
		nxtX = mBox.x - ENTITY_VEL;
		nxtY = mBox.y;
		break;
	case RIGHT:
		nxtX = mBox.x + ENTITY_VEL;
		nxtY = mBox.y;
		break;
	default:
		return;
	}

	int lSup = nxtY / TILE_SIZE; // linha superior
	int lInf = (nxtY + mBox.h - 1) / TILE_SIZE; // linha inferior
	int cEsq = nxtX / TILE_SIZE; // coluna esquerda
	int cDir = (nxtX + mBox.w - 1) / TILE_SIZE; // coluna direita

	// verifica se existe uma parede em cada canto
	bool collision = (mapa[lSup][cEsq] == 1 || mapa[lInf][cEsq] == 1 || mapa[lSup][cDir] == 1 || mapa[lInf][cDir] == 1);

	if (!collision) { 
		actDirection = nxtDirection;
		setVel();
	}
}

void PacMan::move() {
	mBox.x += mVelX;

	// testa colisão considerando todos os cantos da entidade
	int lSup = mBox.y / TILE_SIZE; // linha superior
	int lInf = (mBox.y + mBox.h - 1) / TILE_SIZE; // linha inferior
	int cEsq = mBox.x / TILE_SIZE; // coluna esquerda
	int cDir = (mBox.x + mBox.w - 1) / TILE_SIZE; // coluna direita

	// verifica se existe uma parede em cada canto
	bool collision = (mapa[lSup][cEsq] == 1 || mapa[lInf][cEsq] == 1 || mapa[lSup][cDir] == 1 || mapa[lInf][cDir] == 1);
	if ((cEsq < 0 && lSup == 17) && actDirection == LEFT) {
		mBox.x = SCREEN_WIDTH - mBox.w;
	}	
	else if ((cDir >= MAP_WIDTH && lSup == 17) && actDirection == RIGHT) {
		mBox.x = 0 + mBox.w;
	}

	if (collision) {
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

