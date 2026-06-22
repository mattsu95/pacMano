#include "LTexture.h"
#include "Entity.h"

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture() {
	free();
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer* gRenderer) {
	free();

	SDL_Texture* newTexture = NULL;

	// carrega imagem
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) { printf("Nao foi possivel carregar a imagem %s! SDL_Image error: %s\n", path.c_str(), SDL_GetError()); }
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		// cria textura com os pixels da superficie
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)	{ printf("Nao foi possivel criar a textura de %s! SDL Error: %s\n", path.c_str(), SDL_GetError()); }
		else {
			// dimensoes da imagem
			mWidth = Entity::ENTITY_WIDTH;
			mHeight = Entity::ENTITY_HEIGHT;
		}

		SDL_FreeSurface(loadedSurface);
	}
	
	mTexture = newTexture;
	return mTexture != NULL;
}

void LTexture::render(int x, int y, SDL_Renderer* gRenderer) {
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