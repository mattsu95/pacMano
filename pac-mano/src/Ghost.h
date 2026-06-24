#pragma once
#include "Entity.h"

class Ghost : public Entity {
public:
	// using Entity::Entity;

	Ghost(LTexture* texture);

	void move();

	bool followRoute(std::vector<std::pair<int, int>>* route, SDL_Rect pacBox);

	void resetRoute();

private:
	std::pair<int, int> nxtPos;
};
