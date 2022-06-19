#include "PhysicsObject.h"

bool IsOdd(float x) {
	int n = floor(x);
	return n % 2 == 1;
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
	Vector2 windowSize = Vector2(gameview.getSize().x, gameview.getSize().y);
	
	//Applies gravity or friction ALL THE TIME
	moveSpeed += gravityAndFriction * dt;

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
	bool isMirroredX = IsOdd(position.x / windowSize.x);
	bool isMirroredY = IsOdd(position.y / windowSize.y);
	
	//If we have gone out of bounds, bring us back :)
	position = position % windowSize;
	
	//If we bounced off the side fix position and invert speed (Welcome in the mirror world!)
	if (isMirroredX) {
		moveSpeed.x = -moveSpeed.x;
		position.x = gameview.getSize().x - position.x;
	}
	if (isMirroredY) {
		moveSpeed.y = -moveSpeed.y;
		position.y = gameview.getSize().y - position.y;
	}
}

void PhysicsObject::ApplyForce(Vector2 instantAcceleration) {
	this->moveSpeed += instantAcceleration;
}

