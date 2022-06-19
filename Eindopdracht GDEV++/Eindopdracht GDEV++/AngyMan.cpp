#include "AngyMan.h"

AngyMan::AngyMan()
{
}

AngyMan::~AngyMan()
{
}


void spawnEnemy(std::list <AngyMan*>& angymanList, sf::Texture& texture, Vector2 windowWH) {
	//create a new angyman
	AngyMan* enemy = new AngyMan();
	enemy->body.setTexture(texture);
		
	enemy->SetWidthHeight(Vector2(enemy->GetWidthHeight().x * 0.25f, enemy->GetWidthHeight().y * 0.2f));

	enemy->gravity = Vector2(0, 100);
	enemy->friction = Vector2(0, 0);
	enemy->fixBounds();
	int boundsCorrector = std::abs((unsigned int)windowWH.x - enemy->GetWidthHeight().x);
	enemy->SetPos(Vector2(rand() % boundsCorrector, -enemy->GetWidthHeight().y));
	enemy->body.setColor(sf::Color(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100));

	enemy->moveSpeed = Vector2(0,0);
	angymanList.push_back(enemy);
}