#include "GuiComponent.h"
#include "Box2dTransform.h"
#include "FerryShipController.h"
#include "MiningBaseController.h"

void GuiComponent::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	this->entityWindow = tgui::ChildWindow::create();
	this->entityTabs = tgui::Tabs::create();
	this->entityTabs->connect("TabSelected", &GuiComponent::changePanel, this);
	this->entityWindow->add(this->entityTabs);
	this->update();
	this->entityTabs->select(0);
}
void GuiComponent::update() {
	this->entityWindow->setTitle(this->getParent().lock()->getStringRep());
	this->entityWindow->removeAllWidgets();
	this->entityWindow->add(this->entityTabs);
	this->entityTabs->removeAll();
	this->entityPanels = {};
	for (ComponentType c : ComponentList::allTypes) {
		if (std::shared_ptr<Component> comp = this->getParent().lock()->components.get(c)) {
			comp->updateGui(this->entityTabs, &this->entityPanels);
		}
	}
	for (auto it = this->entityPanels.begin(); it != this->entityPanels.end(); ++it) {
		it->second->setSize({
			this->entityWindow->getFullSize().x,
			this->entityWindow->getFullSize().y - this->entityTabs->getFullSize().y});
		it->second->setPosition({ 0, this->entityTabs->getFullSize().y });
	}
	// Reselect the panel that was selected
	if (this->selectedPanel != "") {
		this->changePanel(this->selectedPanel);
	}
}

void GuiComponent::changePanel(std::string selectedPanel) {
	if (this->selectedPanel != "") {
		this->entityWindow->remove(this->entityPanels[this->selectedPanel]);
	}
	this->entityTabs->select(selectedPanel);
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
void GuiComponent::onClick() {
	if (!this->entityWindow->getParent()) {
		this->show();
	}
}
void GuiComponent::show() {
	this->getParent().lock()->game->getGui()->add(this->entityWindow);
	this->entityWindow->setPosition(
		sf::Vector2f(this->getParent().lock()->game->getWindow()->mapCoordsToPixel(
			this->getParent().lock()->components.transform->getPosition()
		))
	);
}
void GuiComponent::hide() {
	this->getParent().lock()->game->getGui()->remove(this->entityWindow);
}