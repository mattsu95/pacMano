#pragma once
#include "Entity.h"

class Ghost : public Entity {
public:
	// using Entity::Entity;

	Ghost(LTexture* texture);

	void move();

	bool checkDirection(std::vector<std::pair<int, int>>* route);

private:
	std::pair<int, int> nxtPos;
};
