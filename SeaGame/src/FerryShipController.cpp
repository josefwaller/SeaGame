#include "FerryShipController.h"
#include "BaseController.h"
#include "InventoryComponent.h"

FerryShipController::FerryShipController(std::weak_ptr<Entity> parent): AutomatedShipController(parent) {
}
void FerryShipController::update(float delta) {
	// Move towards destination if it exists
	if (this->destination.lock()) {
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
}
void FerryShipController::setStopDropOff(size_t stopIndex, GameResource res, bool val) {
	this->stops[stopIndex].toDropOff[res] = val;
}
sf::Vector2f FerryShipController::getCoordsForEntity(std::weak_ptr<Entity> e) {
	if (auto b = std::dynamic_pointer_cast<BaseController>(e.lock()->components.controller)) {
		return b->getDockCoords();
	}
	else {
		return e.lock()->components.transform->getPosition();
	}
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
		label->setText(std::to_string(it->target.lock()->tag) + " (" + std::to_string(it->target.lock()->team) + ")");
		lay->add(label);
		// Add menu for what to pick up/drop off
		tgui::Button::Ptr exchangeButton = tgui::Button::create();
		exchangeButton->setText("Pick up/drop off");
		exchangeButton->connect("clicked", [&](Game* g, size_t index, std::weak_ptr<FerryShipController> cont, FerryShipController::FerryStop stop) {
			// Add the window
			tgui::ChildWindow::Ptr exchangeWindow = tgui::ChildWindow::create();
			// Add a vertical layout for th window
			tgui::VerticalLayout::Ptr excLayout = tgui::VerticalLayout::create();
			exchangeWindow->add(excLayout);
			// ToDo: for each stop, choose what resources to take from it depending on its type
			// Currently just use this list
			std::vector<GameResource> resources = {
				GameResource::Wood,
				GameResource::Plank,
				GameResource::Stone,
				GameResource::StoneBrick,
				GameResource::StoneStatue
			};
			for (auto res : resources) {
				tgui::Button::Ptr pickup = tgui::Button::create();
				bool isPickingUp = stop.toPickUp.find(res) != stop.toPickUp.end() && stop.toPickUp[res];
				pickup->setText((isPickingUp ? "Dont' pick up " : "Pick up ") + getResourceString(res));
				pickup->connect("clicked", [&](
					std::weak_ptr<FerryShipController> cont,
					size_t index,
					GameResource res,
					bool value
					) {
					cont.lock()->setStopPickUp(index, res, !isPickingUp);
					cont.lock()->getParent().lock()->components.gui->update();
				}, cont, index, res, true);
				excLayout->add(pickup);
				bool isDroppingOff = stop.toDropOff.find(res) != stop.toDropOff.end() && stop.toPickUp[res];
				tgui::Button::Ptr dropOff = tgui::Button::create();
				dropOff->setText((isDroppingOff ? "Don't drop off " : "Drop off ") + getResourceString(res));
				dropOff->connect("clicked", [&](
					std::weak_ptr<FerryShipController> cont,
					size_t index,
					GameResource res,
					bool value
					) {
					cont.lock()->setStopDropOff(index, res, value);
					cont.lock()->getParent().lock()->components.gui->update();
				}, cont, index, res, !isDroppingOff);
				excLayout->add(dropOff);
			}
			// Add window to gui
			g->getGui()->add(exchangeWindow);
		}, this->getParent().lock()->game, index, controller, *it);
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
	}, this->getParent().lock()->game, controller);
	layout->add(addStopButton);

}

std::map<std::string, std::string> FerryShipController::getSaveData() {
	std::map<std::string, std::string> toReturn;
	std::string stopCount = std::to_string(this->stops.size());
	toReturn["stopCount"] = stopCount;
	for (auto it = this->stops.begin(); it != this->stops.end(); it++) {
		toReturn["stop_" + std::to_string(std::distance(this->stops.begin(), it))] = std::to_string(it->target.lock()->id);
	}
	toReturn["currentIndex"] = std::to_string(this->currentStopIndex);
	return toReturn;
}

void FerryShipController::fromSaveData(std::map<std::string, std::string> data) {
	size_t count = std::stoi(data["stopCount"]);
	for (size_t i = 0; i < count; i++) {
		unsigned int id = std::stoi(data["stop_" + std::to_string(i)]);
		this->stops.push_back({
			this->getParent().lock()->game->getEntityById(id),
			{},
			{}
		});
	}
	this->currentStopIndex = std::stoi(data["currentIndex"]);
	if (this->stops.size() > 0) {
		this->destination = this->stops[this->currentStopIndex].target;
		// Have to set target, so that it will create points for the ship to go to
		this->setTarget(this->getCoordsForEntity(this->destination));
	}
	this->getParent().lock()->components.gui->update();
}