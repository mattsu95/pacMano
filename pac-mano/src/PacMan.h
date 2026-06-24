#pragma once
#include "Entity.h"

class PacMan : public Entity {
public:
	// using Entity::Entity;
	
	PacMan(LTexture* texture);

	void handleEvent(SDL_Event& e);

	void move() override;
};