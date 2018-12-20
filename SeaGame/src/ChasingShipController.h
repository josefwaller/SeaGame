#pragma once
#include "ShipController.h"

class ChasingShipController : public ShipController
{
public:
	virtual void update(float delta) override;

	void setTarget(std::weak_ptr<Entity>);

	// The angle (in radians) where the ship will stop turning to face the target
	// i.e. When the ChasingShip is facing the target within x radians, it will stop turning
	static const float ANGLE_TURN_MARGIN;
	// The angle difference between the line to the target and the ship's current target needed for the ship to move
	// i.e. The ship will not move unless it is facing the target within x radians
	static const float ANGLE_MOVE_MARGIN;
	// The distance at which the ship will stop moving and just fire upon the target
	static const float MIN_MOVE_DISTANCE;
	// The distance needed to chase a target
	static const float MAX_CHASE_RANGE;
private:
	std::weak_ptr<Entity> target;
};