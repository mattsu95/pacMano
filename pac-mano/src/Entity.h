#pragma once
#include "LTexture.h"

class Entity {
public:
	Entity(LTexture* texture);

	// atributos gerais
	static const int ENTITY_WIDTH = 15;
	static const int ENTITY_HEIGHT = 15;
	static const int ENTITY_VEL = 1;

	// métodos
	virtual void move();

	void render(SDL_Renderer* gRenderer);

	inline SDL_Rect getBox() { return mBox; }

	void setVel();

protected:
	Direction actDirection; // direção atual
	Direction nxtDirection; // próxima direção
	SDL_Rect mBox;			// hitbox
	int mVelX, mVelY;		// velocidade horizontal e vertical
	LTexture* mTexture;		// textura
};
