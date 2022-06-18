#pragma once
#include "Actor.h"
class AngyMan :
    public Actor
{
	public:
	AngyMan();
	~AngyMan();
	void Update(float dt) override;
};

