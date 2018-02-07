#include "ShipHealth.h"

ShipHealth::ShipHealth(std::weak_ptr<Entity> parent) : HealthComponent(parent)
{
	this->addHealth(HealthType::Sails, 100);
	this->addHealth(HealthType::Hull, 100);
}