#pragma once
#include "Component.h"

class ControllerComponent : public Component
{
public:
	ControllerComponent(std::weak_ptr<Entity> e);
	virtual void update(float delta) = 0;
};