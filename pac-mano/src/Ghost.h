#pragma once
#include "Entity.h"

class Ghost : public Entity {
public:
	using Entity::Entity;

	void move();
};
