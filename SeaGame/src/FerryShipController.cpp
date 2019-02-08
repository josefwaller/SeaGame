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
	this->updateComboBox();
}
void FerryShipController::removeStop(size_t i) {
	this->stops.erase(this->stops.begin() + i);
	this->currentStopIndex--;
	this->updateComboBox();
}
// Move the stop at currentOrder to newOrder and shift the other stops down by one
void FerryShipController::setStopOrder(size_t currentOrder, size_t newOrder) {
	// Copy
	FerryStop stop = this->stops[currentOrder];
	// Remove
	this->stops.erase(this->stops.begin() + currentOrder);
	// Reinsert
	this->stops.insert(this->stops.begin() + newOrder, stop);
	this->updateComboBox();
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
std::weak_ptr<FerryShipController> FerryShipController::getWeakPtr() {
	return std::dynamic_pointer_cast<FerryShipController>(this->getParent().lock()->components.controller);
}
void FerryShipController::updatePanel() {
	// Add the this->panel
	this->panel->setSize(
		GuiComponent::WINDOW_WIDTH,
		250
	);
	this->panel->setHorizontalScrollbarPolicy(tgui::ScrollablePanel::ScrollbarPolicy::Never);
	auto cb = std::dynamic_pointer_cast<tgui::ComboBox>(this->panel->get("StopComboBox"));
	if (cb->getSelectedItem() != "") {
		this->updateTransferPanel(std::stoi(std::string(cb->getSelectedItemId())));
	}
	// Add new stop button
	tgui::Button::Ptr addStopButton = tgui::Button::create();
	addStopButton->setText("Add new stop");
	addStopButton->connect("clicked", [&](Game* g, std::weak_ptr<FerryShipController> cont) {
		// Select the entity to add a stop to
		g->getHud()->selectEntity(std::bind([&](std::weak_ptr<Entity> e, std::weak_ptr<FerryShipController> c) {
			c.lock()->addStop(e);
		}, std::placeholders::_1, cont));
	}, this->getGame(), getWeakPtr());
	addStopButton->setPosition(0, this->stops.size() * 100);
	addStopButton->setSize(GuiComponent::WINDOW_WIDTH, 50);
	this->panel->add(addStopButton);
}
void FerryShipController::updateComboBox() {
	// Add box of each stop
	std::vector<FerryStop> stops = this->getStops();
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
	}, getWeakPtr());
	if (selectedItem != "") {
		this->updateTransferPanel(std::stoi(selectedItem));
	}
}
void FerryShipController::updateTransferPanel(size_t index) {
	// Clear out the stop container
	auto g = std::dynamic_pointer_cast<tgui::HorizontalLayout>(this->panel->get("StopTransferGroup"));
	g->removeAllWidgets();
	// Get the selected stop
	FerryStop stop = this->stops[index];
	// Add the pickup panel, with the method to add the resource to the stop's pick up
	g->add(getTransferGui("Pick Up", stop.toPickUp, index, &FerryShipController::setStopPickUp));
	// Add the drop off panel too
	g->add(getTransferGui("Drop Off", stop.toDropOff, index, &FerryShipController::setStopDropOff));
}

tgui::Group::Ptr FerryShipController::getTransferGui(
	std::string titleStr,
	std::map<GameResource, bool> resources,
	size_t stopIndex,
	std::function<void(FerryShipController*, size_t, GameResource, bool)> callback) {
	auto toReturn = tgui::Group::create();
	auto title = tgui::Label::create();
	title->setText(titleStr);
	toReturn->add(title);
	// Add labels for each thing to pick up
	float h = title->getSize().y;
	for (auto pair : resources) {
		if (pair.second) {
			auto l = tgui::Label::create();
			std::string s = getResourceString(pair.first);
			l->setText(s);
			l->setPosition(0, h);
			h += l->getSize().y;
			toReturn->add(l);
		}
	}
	// Add a combo box to choose what to pick up/drop off
	auto cb = tgui::ComboBox::create();
	cb->setPosition(0, h);
	toReturn->add(cb);
	for (GameResource res : ALL_RESOURCES) {
		if (!resources[res])
			cb->addItem(getResourceString(res), std::to_string(res));
	}
	// Add a resource when selected
	auto c = getWeakPtr();
	cb->connect("ItemSelected", [stopIndex, callback, c](
		sf::String name,
		sf::String id
		) {
		if (c.lock()) {
			GameResource res = (GameResource)(std::stoi(std::string(id)));
			callback(c.lock().get(), stopIndex, res, true);
		}
	});
	cb->setItemsToDisplay(3);
	return toReturn;
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
	this->updatePanel();
	this->updateComboBox();
}