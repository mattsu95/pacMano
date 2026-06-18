#include "Entity.h"

Entity::Entity(LTexture* texture) {
	mBox.x = 0;
	mBox.y = 0;

	mVelX = 0;
	mVelY = 0;

	mTexture = texture;
}

void Entity::move(const int SCREEN_WIDTH, const int SCREEN_HEIGHT){}

void Entity::render(SDL_Renderer* gRenderer) {
	mTexture->render(mBox.x, mBox.y, gRenderer);
}