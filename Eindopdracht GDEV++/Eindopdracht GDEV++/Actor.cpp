#include "Actor.h"

//collision check
bool Actor::CheckCollision(Actor* B)
{
	return (
		this->position.x + this->widthHeight.x > B->position.x
		&& this->position.x < B->position.x + B->widthHeight.x
		&& this->position.y + this->widthHeight.y > B->position.y
		&& this->position.y < B->position.y + B->widthHeight.y
	);
}


void Actor::SetPos(Vector2 pos)
{
	this->position = Vector2(pos.x, pos.y);
	body.setPosition(this->position.x, this->position.y);
}


Vector2 Actor::GetPos()
{
	return position;
}

void Actor::fixBounds()
{
	body.setScale(
		sf::Vector2f(
			widthHeight.x / body.getTexture()->getSize().x, 
			widthHeight.y / body.getTexture()->getSize().y
		)
	);
}
