#pragma once
#include "AutomatedShipController.h"

class FerryShipController : public AutomatedShipController {
public:
	FerryShipController(std::weak_ptr<Entity> parent);
	virtual void update(float delta) override;
	std::vector<std::weak_ptr<Entity>> getStops();
	void addStop(std::weak_ptr<Entity> stop);
	void setStopOrder(size_t currentOrder, size_t newOrder);
	virtual std::map<std::string, std::string> getSaveData() override;
	virtual void fromSaveData(std::map<std::string, std::string> data) override;
private:
	// Current destination the ship is going to
	std::weak_ptr<Entity> destination;
	std::vector<std::weak_ptr<Entity>> stops;
	size_t currentStopIndex;
	virtual void onReachingTarget() override;
	sf::Vector2f getCoordsForEntity(std::weak_ptr<Entity> e);
};