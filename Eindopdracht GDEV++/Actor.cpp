#include "Actor.h"

//collision check between self and other actor
bool Actor::CheckCollision(Actor* B)
{
	return (
		this->position.x + this->widthHeight.x > B->position.x
		&& this->position.x < B->position.x + B->widthHeight.x
		&& this->position.y + this->widthHeight.y > B->position.y
		&& this->position.y < B->position.y + B->widthHeight.y
	);
}

//Sets the position for both the actor and the sprite
void Actor::SetPos(Vector2 pos)
{
	this->position = Vector2(pos.x, pos.y);
	body.setPosition(this->position.x, this->position.y);
}

//Returns position
Vector2 Actor::GetPos()
{
	return position;
}

//This makes sure the sprite matches it's colissionary boundaries
void Actor::fixBounds()
{
	body.setScale(
		sf::Vector2f(
			widthHeight.x / body.getTexture()->getSize().x, 
			widthHeight.y / body.getTexture()->getSize().y
		)
	);
}

