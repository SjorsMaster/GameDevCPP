#pragma once
#include "Maths.h"
#include <SFML\Graphics.hpp>

class Actor
{
public:
	virtual void Update(float dt);
	void SetPos(Vector2 pos);
	Vector2 WidthHeight;
	Vector2 GetPos();
	float moveSpeed = 0;
	float speed = 1;
	const float acceleration = 100.0f;
	Vector2 position;
	Vector2 widthHeight = Vector2(160,160);
	void fixBounds();

	sf::Sprite body;

/*
	float velocity; // Snelheid
	float acceleration; // Hoe veel de snelheid hoger wordt per seconde/frame/timestep/whatever
	Vector2 position;
	Vector2 direction; // Vector2(0, -1);

	void Update();
*/

};
