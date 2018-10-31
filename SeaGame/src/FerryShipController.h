#pragma once
#include "AutomatedShipController.h"

class FerryShipController : public AutomatedShipController {
public:
	struct FerryStop {
		std::weak_ptr<Entity> target;
		std::map<GameResource, bool> toPickUp;
		std::map<GameResource, bool> toDropOff;
	};
	FerryShipController(std::weak_ptr<Entity> parent);
	virtual void update(float delta) override;
	std::vector<FerryStop> getStops();
	// Add a stop
	void addStop(std::weak_ptr<Entity> stop);
	// Set the position of one stop, and shift all the stops after down by 1
	void setStopOrder(size_t currentOrder, size_t newOrder);
	// Set what to pick up/drop off at the stop
	void setStopPickUp(size_t stopIndex, GameResource res, bool shouldPickUp);
	void setStopDropOff(size_t stopIndex, GameResource res, bool shouldDropOff);
	virtual std::map<std::string, std::string> getSaveData() override;
	void updateGui(tgui::Tabs::Ptr tabs, std::map<std::string, tgui::Panel::Ptr>* panels) override;
	virtual void fromSaveData(std::map<std::string, std::string> data) override;
private:
	// Current destination the ship is going to
	std::weak_ptr<Entity> destination;
	std::vector<FerryStop> stops;
	size_t currentStopIndex;
	virtual void onReachingTarget() override;
	sf::Vector2f getCoordsForEntity(std::weak_ptr<Entity> e);
};