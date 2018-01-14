#pragma once
#include "Component.h"

class ControllerComponent : public Component
{
public:
	ControllerComponent(Entity& parent);

	virtual void update(unsigned int delta) = 0;
};