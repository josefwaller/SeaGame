#include "ShipHealth.h"

ShipHealth::ShipHealth(std::weak_ptr<Entity> parent) : HealthComponent(parent)
{
	this->addHealth(HealthType::Sails, 1);
	// TEMOPORARILY MAKE THE SHIPS INVINCIBLE
	// DO NOT FORGET TO TAKE OUT
	// ToDo: Take this out
	//this->addHealth(HealthType::Default, 100);
}