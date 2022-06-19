#pragma once
#include "Actor.h"
class AngyMan :
    public Actor
{
	public:
	AngyMan();
	~AngyMan();
	//void Update(float dt) override;
};

void spawnEnemy(std::list<AngyMan*>& angymanList, sf::Texture& texture, Vector2 windowWH);
