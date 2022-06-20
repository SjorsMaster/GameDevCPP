#pragma once
#include "Vector2.h"
#include "PhysicsObject.h"
#include <SFML\Graphics.hpp>

class Actor : public PhysicsObject
{
public:
	bool CheckCollision(Actor* B);
	void SetPos(Vector2 pos);
	Vector2 GetPos();
	void fixBounds();

	sf::Sprite body;
};

