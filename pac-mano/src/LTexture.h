#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

class LTexture {
public:
	LTexture();

	~LTexture();

	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

	// deallocates texture
	void free();

	// renders texture at a given position
	void render(int x, int y, SDL_Renderer* gRenderer);

	// getters
	inline int getWidth() { return mWidth; }
	inline int getHeight() { return mHeight; }

private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};
