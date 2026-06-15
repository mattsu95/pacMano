#pragma once
#include "LTexture.h"

class PacMan {
public:
	static const int DOT_WIDTH = 20;
	static const int DOT_HEIGHT = 20;
	static const int DOT_VEL = 2;

	PacMan(LTexture* texture);

	void handleEvent(SDL_Event& e);

	void move(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);

	void render(SDL_Renderer* gRenderer);

private:
	int mPosX, mPosY;
	int mVelX, mVelY;
	LTexture* mTexture;
};