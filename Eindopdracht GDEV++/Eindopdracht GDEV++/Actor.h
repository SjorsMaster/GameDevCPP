#pragma once
#include "Maths.h"
#include "PhysicsObject.h"
#include <SFML\Graphics.hpp>

class Actor : public PhysicsObject
{
public:
	virtual void Update(float dt);
	bool CheckCollision(Actor* B);
	void SetPos(Vector2 pos);
	Vector2 GetPos();
	//const float acceleration = 100.0f;
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
