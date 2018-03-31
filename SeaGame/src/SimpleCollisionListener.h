#pragma once
#include "Box2D\Box2D.h"

class SimpleCollisionListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact);
};