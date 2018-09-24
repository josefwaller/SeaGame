#include "ShipHealth.h"

ShipHealth::ShipHealth(std::weak_ptr<Entity> parent) : HealthComponent(parent)
{
	this->addHealth(HealthType::Sails, 1);
}