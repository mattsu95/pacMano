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