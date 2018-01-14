#pragma once
#include "Component.h"

class ControllerComponent : public Component
{
public:
	ControllerComponent(Entity& e);
	virtual void update(float delta) = 0;
};