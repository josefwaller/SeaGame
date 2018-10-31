#include "GuiComponent.h"
#include "Box2dTransform.h"
#include "FerryShipController.h"
#include "MiningBaseController.h"

GuiComponent::GuiComponent(std::weak_ptr<Entity> parent) : Component(parent) {
	this->entityWindow = tgui::ChildWindow::create();
	this->selectedPanel = "Inventory";
	this->entityTabs = tgui::Tabs::create();
	this->entityTabs->add("Inventory");
	this->entityTabs->add("Health");
	this->entityPanels["Inventory"] = tgui::Panel::create();
	this->entityPanels["Health"] = tgui::Panel::create();
	this->entityPanels["Inventory"]->setPosition({ 0, this->entityTabs->getFullSize().y });
	this->entityTabs->connect("TabSelected", &GuiComponent::changePanel, this);
	this->entityWindow->add(this->entityTabs);
	this->entityWindow->add(this->entityPanels["Inventory"]);
	if (std::dynamic_pointer_cast<FerryShipController>(this->getParent().lock()->components.controller)) {
		this->entityPanels["Ferry"] = tgui::Panel::create();
		this->entityTabs->add("Ferry");
	}

	this->update();
}
void GuiComponent::update() {
	this->updateInventory();
	if (auto sharedController = std::dynamic_pointer_cast<FerryShipController>(this->getParent().lock()->components.controller)) {
		this->entityPanels["Ferry"]->removeAllWidgets();
		// Make sure to pass weak_ptr as parameter otherwise controller will not just have one owner
		std::weak_ptr<FerryShipController> controller = sharedController;
		// Add the layout
		tgui::VerticalLayout::Ptr layout = tgui::VerticalLayout::create();
		this->entityPanels["Ferry"]->add(layout);
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
	this->entityTabs->select(this->selectedPanel);

}

void GuiComponent::changePanel(std::string selectedPanel) {
	this->entityWindow->remove(this->entityPanels[this->selectedPanel]);
	this->selectedPanel = selectedPanel;
	this->entityWindow->add(this->entityPanels[this->selectedPanel]);
	this->entityPanels[this->selectedPanel]->setPosition({ 0, this->entityTabs->getFullSize().y });
}
std::string GuiComponent::getResourceString(GameResource res) {
	switch (res) {
	case GameResource::Gold: return "Gold";
	case GameResource::Wood: return "Wood";
	case GameResource::Stone: return "Stone";
	case GameResource::Plank: return "Plank";
	case GameResource::StoneBrick: return "Stone Bricks";
	case GameResource::StoneStatue: return "Statue";
	}
	return "N/A";
}


void GuiComponent::updateInventory() {
	this->entityPanels["Inventory"]->removeAllWidgets();
	float y = 0.0f;
	// Add a button for each of the resources
	if (this->getParent().lock()->components.inventory != nullptr) {
		for (auto it : this->getParent().lock()->components.inventory->getInventory()) {
			tgui::Button::Ptr btn = tgui::Button::create();
			btn->setText(getResourceString(it.first) + ": " + std::to_string(it.second));
			btn->setPosition({ 0.0f, y });
			// Make it transfer resources when clicked
			btn->connect("clicked",
				[&](Game* g, std::weak_ptr<Entity> e, GameResource res, unsigned int amount) {
					e.lock()->game->getHud()->transferItems(e, res, amount);
				},
				this->getParent().lock()->game,
				this->getParent(),
				it.first,
				it.second);
			this->entityPanels["Inventory"]->add(btn);
			y += btn->getFullSize().y;
		}
	}
}

void GuiComponent::onClick() {
	if (!this->entityWindow->getParent()) {
		this->getParent().lock()->game->getGui()->add(this->entityWindow);
	}
}
void GuiComponent::show() {
	this->getParent().lock()->game->getGui()->add(this->entityWindow);
}
void GuiComponent::hide() {
	this->getParent().lock()->game->getGui()->remove(this->entityWindow);
}