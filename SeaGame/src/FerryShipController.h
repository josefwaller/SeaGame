#pragma once
#include "AutomatedShipController.h"

class FerryShipController : public AutomatedShipController {
public:
	FerryShipController(std::weak_ptr<Entity> parent, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to);
	virtual void update(float delta) override;
	void setTargets(std::weak_ptr<Entity> takeFrom, std::weak_ptr<Entity> giveTo);
	enum Action {
		PickingUp,
		DropingOff
	};
private:
	std::weak_ptr<Entity> takeFrom;
	std::weak_ptr<Entity> giveTo;
	FerryShipController::Action currentAction;
	virtual void onReachingTarget() override;
};