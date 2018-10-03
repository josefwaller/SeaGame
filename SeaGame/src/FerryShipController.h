#pragma once
#include "AutomatedShipController.h"

class FerryShipController : public AutomatedShipController {
public:
	FerryShipController(std::weak_ptr<Entity> parent, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to);
	virtual void update(float delta) override;
	void setDestination(std::weak_ptr<Entity> dest);
	void setSource(std::weak_ptr<Entity> source);
	enum Action {
		PickingUp,
		DropingOff
	};
private:
	std::weak_ptr<Entity> source;
	std::weak_ptr<Entity> destination;
	FerryShipController::Action currentAction;
	virtual void onReachingTarget() override;
	sf::Vector2f getCoordsForEntity(std::weak_ptr<Entity> e);
};