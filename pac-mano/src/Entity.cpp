#include "Entity.h"

Entity::Entity(LTexture* texture) {
	mBox.w = TILE_SIZE;
	mBox.h = TILE_SIZE;

	mVelX = 0;
	mVelY = 0;

	mTexture = texture;
}

void Entity::move(){}

void Entity::render(SDL_Renderer* gRenderer) {
	mTexture->render(mBox.x, mBox.y, gRenderer);
}

void Entity::checkDirection() {
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

void Entity::setVel() {
	switch (actDirection) {
	case UP:
		mVelX = 0;
		mVelY = -ENTITY_VEL;
		break;
	case DOWN:
		mVelX = 0;
		mVelY = ENTITY_VEL;
		break;
	case LEFT:
		mVelX = -ENTITY_VEL;
		mVelY = 0;
		break;
	case RIGHT:
		mVelX = ENTITY_VEL;
		mVelY = 0;
		break;
	}
}

void Entity::pause() {
	if (!paused) {
		mVelX = mVelY = 0;
		paused = true;
	}
	else {
		setVel();
		paused = false;
	}
}