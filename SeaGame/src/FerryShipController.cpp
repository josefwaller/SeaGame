#include "FerryShipController.h"
#include <TGUI/Gui.hpp>
#include "BaseController.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "SaveData.h"

FerryShipController::FerryShipController() {
	this->panel = tgui::ScrollablePanel::create();
	auto cb = tgui::ComboBox::create();
	this->panel->add(cb, "StopComboBox");
	auto transferGroup = tgui::HorizontalLayout::create();
	transferGroup->setPosition(0, tgui::bindHeight(cb));
	this->panel->add(transferGroup, "StopTransferGroup");
}
void FerryShipController::update(float delta) {
	// Move towards destination if it exists and there is a path to it
	if (this->destination.lock() && !this->points.empty()) {
		this->move(delta);
	}
	else if (this->stops.size() > 0) {
		// Skip this stop
		// Todo: add a warnign to the player here?
		this->currentStopIndex = (this->currentStopIndex + 1) % this->stops.size();
		this->destination = this->stops[this->currentStopIndex].target;
		this->setTarget(this->getCoordsForEntity(this->destination));
	}
}
std::vector<FerryShipController::FerryStop> FerryShipController::getStops() {
	return this->stops;
}
void FerryShipController::addStop(std::weak_ptr<Entity> stop) {
	this->stops.push_back({
		stop,
		{},
		{}
	});
	this->updatePanel();
}
void FerryShipController::removeStop(size_t i) {
	this->stops.erase(this->stops.begin() + i);
	this->currentStopIndex--;
	this->updatePanel();
}
// Move the stop at currentOrder to newOrder and shift the other stops down by one
void FerryShipController::setStopOrder(size_t currentOrder, size_t newOrder) {
	// Copy
	FerryStop stop = this->stops[currentOrder];
	// Remove
	this->stops.erase(this->stops.begin() + currentOrder);
	// Reinsert
	this->stops.insert(this->stops.begin() + newOrder, stop);
	this->updatePanel();
}
void FerryShipController::onReachingTarget() {
	auto destInv = this->destination.lock()->components.inventory;
	auto thisInv = this->getParent().lock()->components.inventory;
	// Get the destination's inventory
	auto destInvData = destInv->getInventory();
	// Get this ship's inventory
	auto thisInvData = thisInv->getInventory();
	// Take things from the  destination
	for (auto it : this->stops[currentStopIndex].toPickUp) {
		// IF the ship should pick up this resource
		if (it.second) {
			destInv->removeItems(it.first, destInvData[it.first]);
			thisInv->addItems(it.first, destInvData[it.first]);
		}
	}
	for (auto it : this->stops[currentStopIndex].toDropOff) {
		if (it.second) {
			destInv->addItems(it.first, thisInvData[it.first]);
			thisInv->removeItems(it.first, thisInvData[it.first]);
		}
	}
	// Go to next target
	this->currentStopIndex = (this->currentStopIndex + 1) % this->stops.size();
	this->destination = this->stops[this->currentStopIndex].target;
	this->setTarget(this->getCoordsForEntity(this->destination));
	this->updatePanel();
}
void FerryShipController::setStopPickUp(size_t stopIndex, GameResource res, bool val) {
	this->stops[stopIndex].toPickUp[res] = val;
	this->updatePanel();
}
void FerryShipController::setStopDropOff(size_t stopIndex, GameResource res, bool val) {
	this->stops[stopIndex].toDropOff[res] = val;
	this->updatePanel();
}

tgui::Widget::Ptr FerryShipController::getGui() {
	return this->panel;
}
void FerryShipController::updatePanel() {
	// Make sure to pass weak_ptr as parameter otherwise controller will not just have one owner
	std::weak_ptr<FerryShipController> controller = std::dynamic_pointer_cast<FerryShipController>(
		this->getParent().lock()->components.controller
	);
	// Add the this->panel
	this->panel->setSize(
		GuiComponent::WINDOW_WIDTH,
		250
	);
	this->panel->setHorizontalScrollbarPolicy(tgui::ScrollablePanel::ScrollbarPolicy::Never);
	// Add box of each stop
	std::vector<FerryShipController::FerryStop> stops = controller.lock()->getStops();
	auto comboBox = std::dynamic_pointer_cast<tgui::ComboBox>(this->panel->get("StopComboBox"));
	this->panel->add(comboBox);
	std::string selectedItem = comboBox->getSelectedItemId();
	comboBox->removeAllItems();
	for (auto it = stops.begin(); it != stops.end(); it++) {
		// Get index of this stop
		size_t index = std::distance(stops.begin(), it);
		comboBox->addItem(it->target.lock()->getStringRep(), std::to_string(index));
	}
	comboBox->setSelectedItemById(selectedItem);
	// Connect combobox to method to show stop
	comboBox->connect("ItemSelected", [&](std::weak_ptr<FerryShipController> c, sf::String name, sf::String item) {
		if (c.lock() && item != "") {
			c.lock()->updateTransferPanel(std::stoi(std::string(item)));
		}
	}, controller);
	if (selectedItem != "") {
		this->updateTransferPanel(std::stoi(selectedItem));
	}
	// Add new stop button
	tgui::Button::Ptr addStopButton = tgui::Button::create();
	addStopButton->setText("Add new stop");
	addStopButton->connect("clicked", [&](Game* g, std::weak_ptr<FerryShipController> cont) {
		// Select the entity to add a stop to
		g->getHud()->selectEntity(std::bind([&](std::weak_ptr<Entity> e, std::weak_ptr<FerryShipController> c) {
			c.lock()->addStop(e);
		}, std::placeholders::_1, cont));
	}, this->getGame(), controller);
	addStopButton->setPosition(0, this->stops.size() * 100);
	addStopButton->setSize(GuiComponent::WINDOW_WIDTH, 50);
	this->panel->add(addStopButton);
}
void FerryShipController::updateTransferPanel(size_t index) {
// Clear out the stop container
	auto g = std::dynamic_pointer_cast<tgui::HorizontalLayout>(this->panel->get("StopTransferGroup"));
	g->removeAllWidgets();
	// Get the selected stop
	FerryStop stop = this->stops[index];
	auto pickupPanel = tgui::Panel::create();
	g->add(pickupPanel);
	float h = 0.0;
	for (auto pair : stop.toPickUp) {
		auto l = tgui::Label::create();
		std::string s = getResourceString(pair.first);
		l->setText(s);
		l->setPosition(0, h);
		h += l->getSize().y;
		pickupPanel->add(l);
	}
}

SaveData FerryShipController::getSaveData() {
	SaveData toReturn("Component");
	toReturn.addValue("stopCount", std::to_string(this->stops.size()));
	toReturn.addValue("currentIndex", std::to_string(this->currentStopIndex));
	for (auto it = this->stops.begin(); it != this->stops.end(); it++) {
		// Create a SaveData for this stop
		SaveData sd("FerryStop");
		sd.addValue("index", std::to_string(std::distance(this->stops.begin(), it)));
		sd.addValue("target" , std::to_string(it->target.lock()->id));
		for (auto pickUp : it->toPickUp) {
			if (pickUp.second) {
				SaveData pickUpData("PickUp");
				pickUpData.addValue("resource", std::to_string(pickUp.first));
				sd.addData(pickUpData);
			}
		}
		for (auto dropOff : it->toDropOff) {
			if (dropOff.second) {
				SaveData dropOffData("DropOff");
				dropOffData.addValue("resource", std::to_string(dropOff.first));
				sd.addData(dropOffData);
			}
		}
		toReturn.addData(sd);
	}
	return toReturn;
}

void FerryShipController::fromSaveData(SaveData data) {
	size_t count = std::stoi(data.getValue("stopCount"));
	this->stops.resize(count);
	for (SaveData stopData : data.getDatas()) {
		size_t index = std::stoi(stopData.getValue("index"));
		std::map<GameResource, bool> toPickUp;
		std::map<GameResource, bool> toDropOff;
		for (SaveData invData : stopData.getDatas()) {
			if (invData.getName() == "PickUp") {
				toPickUp[(GameResource)std::stoi(invData.getValue("resource"))] = true;
			}
			else if (invData.getName() == "DropOff") {
				toDropOff[(GameResource)std::stoi(invData.getValue("resource"))] = true;
			}
		}
		this->stops[index] = {
			this->getGame()->getEntityById(std::stoi(stopData.getValue("target"))),
			toPickUp,
			toDropOff
		};
	}
	this->currentStopIndex = std::stoi(data.getValue("currentIndex"));
	if (this->stops.size() > 0) {
		this->destination = this->stops[this->currentStopIndex].target;
		// Have to set target, so that it will create points for the ship to go to
		this->setTarget(this->getCoordsForEntity(this->destination));
	}
}