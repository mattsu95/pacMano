#pragma once
#include "LTexture.h"

class Entity {
public:
	Entity(LTexture* texture);

	virtual void move(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);

	void render(SDL_Renderer* gRenderer);

	inline SDL_Rect getBox() { return mBox; }

protected:
	SDL_Rect mBox;
	int mVelX, mVelY;
	LTexture* mTexture;
};
