#pragma once
#include "AutomatedShipController.h"

class ExportShipController : public AutomatedShipController {
public:
	static const sf::Vector2f OFFSCREEN_TARGET;
	ExportShipController(std::weak_ptr<Entity> base);
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void update(float delta) override;
	virtual void onReachingTarget() override;
private:
	// The base that spawned the ship
	std::weak_ptr<Entity> base;
	// Whether the ship is going to the base or offscreen
	bool goingToBase;
};
