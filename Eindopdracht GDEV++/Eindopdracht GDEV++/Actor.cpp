#include "Actor.h"
void Actor::Update(float dt)
{
/*
    //add sprite offset based on width+height

        velocity += acceleration;
        position = position + direction.normalize() * velocity;
*/
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
    body.setScale(sf::Vector2f(widthHeight.x / body.getTexture()->getSize().x, widthHeight.y / body.getTexture()->getSize().y));
}
