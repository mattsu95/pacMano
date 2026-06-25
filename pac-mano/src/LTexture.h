#pragma once
#include "Constants.h"

class LTexture {
public:
	LTexture();

	~LTexture();

	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

	// deallocates texture
	void free();

	// renders texture at a given position
	void render(int x, int y, SDL_Renderer* gRenderer);

	// getters e setters
	inline int getWidth() { return mWidth; }
	inline int getHeight() { return mHeight; }
	inline void setWidth(int width) { mWidth = width; }
	inline void setHeight(int height) { mHeight = height; }

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};
