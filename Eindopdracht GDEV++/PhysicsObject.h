#pragma once
#include "Vector2.h"
#include <SFML/Graphics.hpp>

class PhysicsObject
{
public:
	void SetWidthHeight(Vector2 input);
	void ApplyForce(Vector2 acceleration);
	Vector2 GetWidthHeight();
	void UpdatePhysics(float dt, sf::Window& gameview);
	Vector2 moveSpeed = Vector2(0,0);
	Vector2 gravity = Vector2(0,0);
	Vector2 friction = Vector2(0,0);

protected:
	float gameSpeed = 1;
	float weight = 1;
	Vector2 position;
	Vector2 widthHeight = Vector2(160, 160);
};

