#include "FerryShipController.h"
#include "BaseController.h"
#include "InventoryComponent.h"

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
}
// Move the stop at currentOrder to newOrder and shift the other stops down by one
void FerryShipController::setStopOrder(size_t currentOrder, size_t newOrder) {
	// Copy
	FerryStop stop = this->stops[currentOrder];
	// Remove
	this->stops.erase(this->stops.begin() + currentOrder);
	// Reinsert
	this->stops.insert(this->stops.begin() + newOrder, stop);
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
	this->getParent().lock()->components.gui->update();
}
void FerryShipController::setStopDropOff(size_t stopIndex, GameResource res, bool val) {
	this->stops[stopIndex].toDropOff[res] = val;
	this->getParent().lock()->components.gui->update();
}

void FerryShipController::updateGui(tgui::Tabs::Ptr tabs, std::map<std::string, tgui::Panel::Ptr>* panels) {
	tabs->add("Ferry", false);
	panels->insert({ "Ferry", tgui::Panel::create() });
	// Make sure to pass weak_ptr as parameter otherwise controller will not just have one owner
	std::weak_ptr<FerryShipController> controller = std::dynamic_pointer_cast<FerryShipController>(
		this->getParent().lock()->components.controller
	);
	// Add the layout
	tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
	panels->at("Ferry")->add(layout);
	// Add box of each stop
	std::vector<FerryShipController::FerryStop> stops = controller.lock()->getStops();
	for (auto it = stops.begin(); it != stops.end(); it++) {
		// Get index of this stop
		size_t index = std::distance(stops.begin(), it);
		// Add label
		tgui::HorizontalLayout::Ptr lay = tgui::HorizontalLayout::create();
		// Add stop name
		tgui::Label::Ptr label = tgui::Label::create();
		label->setText(it->target.lock()->getStringRep() + " (team " + std::to_string(it->target.lock()->team) + ")");
		lay->add(label);
		// Add menu for what to pick up/drop off
		tgui::Button::Ptr exchangeButton = tgui::Button::create();
		exchangeButton->setText("Pick up/drop off");
		exchangeButton->connect("clicked", [&](Game* g, size_t index, std::weak_ptr<FerryShipController> cont, FerryShipController::FerryStop stop) {
			// Create a new child window
			tgui::ChildWindow::Ptr window = tgui::ChildWindow::create();
			// Set title of window
			window->setTitle("PickUp/DropOff for " + stop.target.lock()->getStringRep());
			// for each resource, add a layout that looks like this
			// | Resource Name | PickingUp? | DroppingOff? |
			tgui::VerticalLayout::Ptr vLayout = tgui::VerticalLayout::create();
			window->add(vLayout);
			// Add a row for each element
			for (GameResource res: ALL_RESOURCES) {
				// Add horizontal layout
				tgui::HorizontalLayout::Ptr hLayout = tgui::HorizontalLayout::create();
				vLayout->add(hLayout);
				// Add text
				tgui::Label::Ptr t = tgui::Label::create();
				t->setText(getResourceString(res));
				hLayout->add(t);
				// Add picking up button
				tgui::Button::Ptr pickupBtn = tgui::Button::create();
				std::string pickupText = stop.toPickUp[res] ? "Picking Up" : "Not Picking Up";
				pickupBtn->setText(pickupText);
				// When button is clicked, toggle picking up this resource
				pickupBtn->connect("clicked", [&](
					std::weak_ptr<FerryShipController> c,
					size_t i,
					GameResource r,
					bool value) {
					c.lock()->setStopPickUp(i, r, value);
				}, cont, index, res, !stop.toPickUp[res]);
				hLayout->add(pickupBtn);
				// Add dropoff button
				tgui::Button::Ptr dropoffBtn = tgui::Button::create();
				std::string dropoffText = stop.toDropOff[res] ? "Dropping off" : "Not Dropping Off";
				dropoffBtn->setText(dropoffText);
				// When button is clicked, toggle dropping off this resource
				dropoffBtn->connect("clicked", [&](
					std::weak_ptr<FerryShipController> c,
					size_t i,
					GameResource r,
					bool value) {
					c.lock()->setStopDropOff(i, r, value);
				}, cont, index, res, !stop.toDropOff[res]);
				hLayout->add(dropoffBtn);
			}
			// Add window
			g->getGui()->add(window);
		}, this->getGame(), index, controller, *it);
		lay->add(exchangeButton);
		// Add remove button
		tgui::Button::Ptr removeButton = tgui::Button::create();
		removeButton->setText("Remove Stop");
		lay->add(removeButton);
		// Add to larger layout
		layout->add(lay);
	}
	// Add new stop button
	tgui::Button::Ptr addStopButton = tgui::Button::create();
	addStopButton->setText("Add new stop");
	addStopButton->connect("clicked", [&](Game* g, std::weak_ptr<FerryShipController> cont) {
		// Select the entity to add a stop to
		g->getHud()->selectEntity(std::bind([&](std::weak_ptr<Entity> e, std::weak_ptr<FerryShipController> c) {
			c.lock()->addStop(e);
			c.lock()->getParent().lock()->components.gui->update();
		}, std::placeholders::_1, cont));
	}, this->getGame(), controller);
	layout->add(addStopButton);

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
	this->getParent().lock()->components.gui->update();
}