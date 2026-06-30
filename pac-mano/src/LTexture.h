// Matheus Seghatti, Arthur Pivotto, Carlos Eduardo e Lucas Henrique

#pragma once
#include "Constants.h"

class LTexture {
public:
	LTexture();

	~LTexture();

	// carrega a textura a partir de um arquivo
	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

	// cria imagem a partir da string de fonte -> função quase igual à loadFromFile
	bool loadFromRenderedText(std::string textureText, SDL_Renderer* gRenderer, TTF_Font* gFont, ALIGN align);

	// desaloca a textura
	void free();

	// renderiza a textura em uma dada posição da tela
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
