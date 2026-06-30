// Matheus Seghatti, Arthur Pivotto, Carlos Eduardo e Lucas Henrique

#pragma once
#include "Entity.h"

class PacMan : public Entity {
public:
	
	PacMan(LTexture* texture);

	void handleEvent(SDL_Event& e);

	void move() override;
};