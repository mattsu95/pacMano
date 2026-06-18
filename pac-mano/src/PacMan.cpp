#include "PacMan.h"

void PacMan::handleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_UP: 
			mVelY = -PACMAN_VEL;
			mVelX = 0;
			break;
		case SDLK_DOWN: 
			mVelY = PACMAN_VEL; 
			mVelX = 0;
			break;
		case SDLK_LEFT:
			mVelX = -PACMAN_VEL;
			mVelY = 0;
			break;
		case SDLK_RIGHT: 
			mVelX = PACMAN_VEL;
			mVelY = 0;
			break;
		}
	}
}

void PacMan::move(const int SCREEN_WIDTH, const int SCREEN_HEIGHT) {
	mBox.x += mVelX;
	if (mBox.x < 0 || (mBox.x + PACMAN_WIDTH > SCREEN_WIDTH)) {
		// move back
		mBox.x -= mVelX;
	}
	mBox.y += mVelY;
	if (mBox.y < 0 || (mBox.y + PACMAN_HEIGHT > SCREEN_HEIGHT)) {
		// move back
		mBox.y -= mVelY;
	}
}

