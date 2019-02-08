#pragma once
#include "AutomatedShipController.h"

class FerryShipController : public AutomatedShipController {
public:
	struct FerryStop {
		std::weak_ptr<Entity> target;
		std::map<GameResource, bool> toPickUp;
		std::map<GameResource, bool> toDropOff;
	};
	FerryShipController();
	virtual void update(float delta) override;
	std::vector<FerryStop> getStops();
	// Add a stop
	void addStop(std::weak_ptr<Entity> stop);
	void removeStop(size_t stopIndex);
	// Set the position of one stop, and shift all the stops after down by 1
	void setStopOrder(size_t currentOrder, size_t newOrder);
	// Set what to pick up/drop off at the stop
	void setStopPickUp(size_t stopIndex, GameResource res, bool shouldPickUp);
	void setStopDropOff(size_t stopIndex, GameResource res, bool shouldDropOff);
	// Update the part of the panel that shows the stop pick up/drop off
	void updateTransferPanel(size_t stopIndex);
	virtual SaveData getSaveData() override;
	virtual tgui::Widget::Ptr getGui() override;
	virtual void fromSaveData(SaveData data) override;
private:
	// Current destination the ship is going to
	std::weak_ptr<Entity> destination;
	// The stops the ferry goes to
	std::vector<FerryStop> stops;
	// The index of the current stop
	size_t currentStopIndex;
	tgui::ScrollablePanel::Ptr panel;
	// Update the GUI panel
	void updatePanel();
	// Update the combo box
	// Warning: This will close it if the player is currently selecting a stop
	void updateComboBox();
	// Get a weak pointer to itself for use in the GUI
	std::weak_ptr<FerryShipController> getWeakPtr();
	// Method on reaching target, see AutomatedShipController::onReachingTarget
	virtual void onReachingTarget() override;
};