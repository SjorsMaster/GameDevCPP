#include "AngyMan.h"

AngyMan::AngyMan()
{
}

AngyMan::~AngyMan()
{
}


void AngyMan::Update(float dt)
{
	this->Actor::Update(dt);

	this->moveSpeed += this->acceleration * dt;

	//this->position += new Vector2(0, this->moveSpeed);
	this->position.y = this->position.y + this->moveSpeed/* * weight */ * this->speed * dt;;
	//this->position.y = rand() % 500;// <<Works, I suppose we're on a seperate space in memory?
	this->SetPos(this->position);
}