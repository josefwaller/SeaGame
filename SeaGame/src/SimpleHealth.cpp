#include "SimpleHealth.h"

SimpleHealth::SimpleHealth(std::weak_ptr<Entity> parent, int maxHealth) : HealthComponent(parent)
{
	this->addHealth(HealthType::Default, maxHealth);
}