#include "PacMan.h"

PacMan::PacMan(LTexture* texture) {
	mPosX = 0;
	mPosY = 0;

	mVelX = 0;
	mVelY = 0;

	mTexture = texture;
}

void PacMan::handleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_UP: 
			mVelY = -DOT_VEL;
			mVelX = 0;
			break;
		case SDLK_DOWN: 
			mVelY = DOT_VEL; 
			mVelX = 0;
			break;
		case SDLK_LEFT:
			mVelX = -DOT_VEL;
			mVelY = 0;
			break;
		case SDLK_RIGHT: 
			mVelX = DOT_VEL;
			mVelY = 0;
			break;
		}
	}
}

void PacMan::move(const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
	mPosX += mVelX;
	if (mPosX < 0 || (mPosX + DOT_WIDTH > SCREEN_WIDTH)) {
		// move back
		mPosX -= mVelX;
	}
	mPosY += mVelY;
	if (mPosY < 0 || (mPosY + DOT_HEIGHT > SCREEN_HEIGHT)) {
		// move back
		mPosY -= mVelY;
	}
}

void PacMan::render(SDL_Renderer* gRenderer) {
	mTexture->render(mPosX, mPosY, gRenderer);
}