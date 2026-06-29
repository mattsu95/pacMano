#pragma once
#include "Entity.h"

class Ghost : public Entity {
public:
	// using Entity::Entity;

	Ghost(LTexture* texture);

	void move();

	bool followRoute(std::vector<std::pair<int, int>>* route, SDL_Rect pacBox); // define a próxima direção com base na rota calculada

	void resetRoute(); // reinicia nxtPos para evitar erros

	// usado pela poda alfa-beta: define diretamente a proxima direcao do fantasma,
	// ja que esse algoritmo decide passo a passo (nao uma rota inteira como o A*)
	void setNextDirection(Direction direcao) { nxtDirection = direcao; }

private:
	std::pair<int, int> nxtPos;
	std::pair<int, int> prevPos;
};
