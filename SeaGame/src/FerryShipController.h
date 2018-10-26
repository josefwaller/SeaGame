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
	std::vector<std::weak_ptr<Entity>> getStops();
	void addStop(std::weak_ptr<Entity> stop);
	void setStopOrder(size_t currentOrder, size_t newOrder);
	virtual std::map<std::string, std::string> getSaveData() override;
	virtual void fromSaveData(std::map<std::string, std::string> data) override;
private:
	std::weak_ptr<Entity> source;
	std::weak_ptr<Entity> destination;
	std::vector<std::weak_ptr<Entity>> stops;
	FerryShipController::Action currentAction;
	virtual void onReachingTarget() override;
	sf::Vector2f getCoordsForEntity(std::weak_ptr<Entity> e);
};