#pragma once
#include "Entity.h"

class PacMan : public Entity {
public:
	using Entity::Entity;

	static const int PACMAN_WIDTH = 20;
	static const int PACMAN_HEIGHT = 20;
	static const int PACMAN_VEL = 2;

	void handleEvent(SDL_Event& e);

	void move(const int SCREEN_WIDTH, const int SCREEN_HEIGHT) override;
};