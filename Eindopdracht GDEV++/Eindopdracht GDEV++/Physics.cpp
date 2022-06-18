#include "Physics.h"
#include "Actor.h"

//Gravity

//Weight


//collision check
bool CheckCollision(Actor* A, Actor* B)
{
	//collision between the player and the enemy which adds points and inverts the movespeed of the enemy
	return (A->position.x + A->widthHeight.x > B->position.x && A->position.x < B->position.x + B->widthHeight.x && A->position.y + A->widthHeight.y > B->position.y && A->position.y < B->position.y + B->widthHeight.y);
}