#pragma once
#include "BaseController.h"
#include "Cannon.h"

class MilitaryBaseController: public BaseController
{
public:
	// How long between taking shots
	static const float SHOOT_DELAY;
	// How close a ship has to be for that base to shoot at it
	static const float SHOOT_RANGE;

	MilitaryBaseController();
	virtual void update(float delta) override;
	// Get the cannons, used to render them
	std::vector<Cannon> getCannons();
private:
	// Record when the cannon can be shot
	sf::Clock shootTime;
	// The current target the base is shooting at
	std::weak_ptr<Entity> target;
	// The cannons in the base
	std::vector<Cannon> cannons;
};