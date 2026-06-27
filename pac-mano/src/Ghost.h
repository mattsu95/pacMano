#pragma once
#include "Entity.h"

class Ghost : public Entity {
public:
	// using Entity::Entity;

	Ghost(LTexture* texture);

	void move();

	bool followRoute(std::vector<std::pair<int, int>>* route, SDL_Rect pacBox); // define a próxima direção com base na rota calculada

	void resetRoute(); // reinicia nxtPos para evitar erros

private:
	std::pair<int, int> nxtPos;
};
