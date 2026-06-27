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

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Renderer* gRenderer, TTF_Font* gFont, ALIGN align) {
	free();

	// define o alinhamento do texto na tela
	switch (align) {
	case ALIGN::LEFT:
		TTF_SetFontWrappedAlign(gFont, TTF_WRAPPED_ALIGN_LEFT);
		break;
	case ALIGN::CENTER:
		TTF_SetFontWrappedAlign(gFont, TTF_WRAPPED_ALIGN_CENTER);
		break;
	case ALIGN::RIGHT:
		TTF_SetFontWrappedAlign(gFont, TTF_WRAPPED_ALIGN_RIGHT);
		break;
	}
	
	// cor da fonte (branco)
	SDL_Color fColor = { 255, 255, 255, 255 };

	// largura máxima do texto
	int maxWidth = SCREEN_WIDTH;

	// cria a superfície do texto
	SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont, textureText.c_str(), fColor, maxWidth);
	if (textSurface == NULL) { printf("Erro ao renderizar a superficie do texto! SDL_ttf Error: %s\n", TTF_GetError()); }
	else {
		// cria a textura com os pixels da superficie e atribui ao atributo do objeto
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL) { printf("Erro ao criar textura do texto renderizado! SDL Error: %s\n", SDL_GetError()); }
		else {
			// pega as dimensões da imagem
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		
		SDL_FreeSurface(textSurface);
	}

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