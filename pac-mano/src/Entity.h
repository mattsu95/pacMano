#pragma once
#include "LTexture.h"

class Entity {
public:
	Entity(LTexture* texture);

	// atributos gerais
	static const int ENTITY_WIDTH = TILE_SIZE;
	static const int ENTITY_HEIGHT = TILE_SIZE;
	static const int ENTITY_VEL = 2;

	// métodos
	virtual void move();

	void render(SDL_Renderer* gRenderer);

	bool checkDirection(); // verifica se consegue mudar de direção

	void setVel();

	void pause();

	// getters e setters
	inline SDL_Rect getBox() { return mBox; }
	
	inline bool isPaused() { return paused; }

protected:
	Direction actDirection; // direção atual
	Direction nxtDirection; // próxima direção
	SDL_Rect mBox;			// hitbox
	int mVelX, mVelY;		// velocidade horizontal e vertical
	LTexture* mTexture;		// textura
	bool paused = false;	// flag pra pausar
};
