#include "GuiComponent.h"
#include "Box2dTransform.h"
#include "FerryShipController.h"
#include "MiningBaseController.h"

const float GuiComponent::WINDOW_HEIGHT = 500.0f;
const float GuiComponent::WINDOW_WIDTH = 600.0f;

void GuiComponent::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	this->entityWindow = tgui::ChildWindow::create();
	this->entityTabs = tgui::Tabs::create();
	this->panelGroup = tgui::Group::create();
	this->entityTabs->connect("TabSelected", &GuiComponent::changePanel, this);
	this->entityWindow->add(this->entityTabs);
	this->entityWindow->add(this->panelGroup);
	this->entityWindow->setSize(
		GuiComponent::WINDOW_WIDTH,
		GuiComponent::WINDOW_HEIGHT + tgui::bindHeight(this->entityTabs)
	);
	this->panelGroup->setPosition(
		0,
		tgui::bindHeight(this->entityTabs)
	);
	this->update();
	this->entityTabs->select(0);
}
void GuiComponent::update() {
	this->entityWindow->setTitle(this->getParent().lock()->getStringRep());
	this->panelGroup->removeAllWidgets();
	this->entityTabs->removeAll();
	this->entityPanels = {};
	for (ComponentType c : ComponentList::allTypes) {
		if (std::shared_ptr<Component> comp = this->getParent().lock()->components.get(c)) {
			comp->updateGui(this->entityTabs, &this->entityPanels);
		}
	}
	this->panelGroup->setSize(sf::Vector2f(
		GuiComponent::WINDOW_WIDTH,
		GuiComponent::WINDOW_HEIGHT
	));
	// Reselect the panel that was selected
	if (this->selectedPanel != "") {
		this->changePanel(this->selectedPanel);
	}
}

void GuiComponent::changePanel(std::string selectedPanel) {
	if (this->selectedPanel != "") {
		this->panelGroup->remove(this->entityPanels[this->selectedPanel]);
	}
	this->entityTabs->select(selectedPanel);
	this->selectedPanel = selectedPanel;
	this->panelGroup->add(this->entityPanels[this->selectedPanel]);
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
void GuiComponent::onClick() {
	if (!this->entityWindow->getParent()) {
		this->show();
	}
}
void GuiComponent::show() {
	this->getGame()->getGui()->add(this->entityWindow);
	this->entityWindow->setPosition(
		sf::Vector2f(this->getGame()->getWindow()->mapCoordsToPixel(
			this->getParent().lock()->components.transform->getPosition()
		))
	);
}
void GuiComponent::hide() {
	this->getGame()->getGui()->remove(this->entityWindow);
}