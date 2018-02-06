#pragma once
#include "HealthComponent.h"
class SimpleHealth : public HealthComponent
{
public:
	SimpleHealth(std::weak_ptr<Entity> parent, int maxHealth);
private:
};