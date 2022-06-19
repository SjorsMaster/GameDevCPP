#include "AngyMan.h"

AngyMan::AngyMan()
{
}

AngyMan::~AngyMan()
{
}

/*
void AngyMan::Update(float dt)
{
	this->Actor::Update(dt);

	this->moveSpeed += this->acceleration * dt;

	//this->position += new Vector2(0, this->moveSpeed);
	this->position.y = this->position.y + this->moveSpeed * weight * this->speed * dt;;
	//this->position.y = rand() % 500;// <<Works, I suppose we're on a seperate space in memory?
	this->SetPos(this->position);
}
*/


void spawnEnemy(std::list <AngyMan*>& angymanList, sf::Texture& texture, Vector2 windowWH) {
	//create a new angyman
	AngyMan* enemy = new AngyMan();
	enemy->body.setTexture(texture);
		
	enemy->SetWidthHeight(Vector2(0.5f / 2, .85f / 2));

	enemy->gravityAndFriction = Vector2(0, 9.8f);
	enemy->fixBounds();
	int boundsCorrector = std::abs((unsigned int)windowWH.x - enemy->GetWidthHeight().x);
	enemy->SetPos(Vector2(rand() % boundsCorrector, -enemy->GetWidthHeight().y));
	enemy->body.setColor(sf::Color(rand() % 250, rand() % 250, rand() % 250));

	enemy->moveSpeed = Vector2(0,0);
	angymanList.push_back(enemy);
}