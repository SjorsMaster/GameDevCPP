#include "PhysicsObject.h"
#include "Vector2.h"

/// <summary>
/// Cool script that handles a lot of the physics in game.
/// Thanks Byte for teaching me how to do some of the maths in this script! <3
/// </summary>

const bool LimitY = false;

bool IsOdd(float x) {
	int n = floor(x);
	return n % 2 != 0;
}

float ApplyFriction(float friction, float speed, float dt) {
	float negFactor = speed < 0 ? -1 : 1;
	speed *= negFactor;
	float frictionForce = friction * dt;
	if (speed < frictionForce) {
		return 0;
	}
	return (speed - frictionForce) * negFactor;
}

void PhysicsObject::SetWidthHeight(Vector2 input)
{
	this->widthHeight = input;
}

Vector2 PhysicsObject::GetWidthHeight()
{
	return this->widthHeight;
}

void PhysicsObject::UpdatePhysics(float dt, sf::Window& gameview) {
	
	Vector2 worldSize = Vector2(gameview.getSize().x, gameview.getSize().y) - widthHeight;
	
	//Applies gravity ALL THE TIME
	moveSpeed += gravity * dt;
	
	//Applies friction when we're moving, deccelerates to 0
	moveSpeed = Vector2(
		ApplyFriction(friction.x, moveSpeed.x, dt), 
		ApplyFriction(friction.y, moveSpeed.y, dt)
	);

	//Prevent slow moving object from wasting PERFORMANCE AND MY TIME!!!
	if (abs(moveSpeed.x) < 0.01f) {
		moveSpeed.x = 0;
	}
	if (abs(moveSpeed.y) < 0.01f) {
		moveSpeed.y = 0;
	}
	
	//How much it moved in amount of time
	position += moveSpeed * dt;
	
	//Figure out whether we've bounced off the side
	bool isMirroredX = IsOdd(position.x / worldSize.x);
	bool isMirroredY = IsOdd(position.y / worldSize.y);

	//If we have gone out of bounds, bring us back :)
	if (LimitY) {
		position = position.Clamp(worldSize);
	}
	else {
		position.x = position.Clamp(worldSize).x;
	}
	//If we bounced off the side fix position and invert speed (Welcome in the mirror world!)
	if (isMirroredX) {
		moveSpeed.x = -moveSpeed.x;
		position.x = worldSize.x - position.x;
	}
	if (isMirroredY && LimitY) {
		moveSpeed.y = -moveSpeed.y;
		position.y = worldSize.y - position.y;
	}
}

void PhysicsObject::ApplyForce(Vector2 instantAcceleration) {
	this->moveSpeed += instantAcceleration;
}

