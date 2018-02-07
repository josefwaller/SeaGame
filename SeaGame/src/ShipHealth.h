#pragma once
#include "HealthComponent.h"

class ShipHealth : public HealthComponent
{
public:
	ShipHealth(std::weak_ptr<Entity> parent);
};