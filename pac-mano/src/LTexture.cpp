#include "LTexture.h"

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	free();
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer* gRenderer) {
	// get rid of preexisting texture
	free();

	SDL_Texture* newTexture = NULL;

	// load image
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) { printf("Unable to load image %s! SDL_Image error: %s\n", path.c_str(), SDL_GetError()); }
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		// create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)	{ printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError()); }
		else {
			// get image dimensions
			mWidth = loadedSurface->w / 10;
			mHeight = loadedSurface->h / 10;
		}

		SDL_FreeSurface(loadedSurface);
	}
	
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::render(int x, int y, SDL_Renderer* gRenderer) {
	// set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

void LTexture::free() {
	if (mTexture != NULL) {
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}