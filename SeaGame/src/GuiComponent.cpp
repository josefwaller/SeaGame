#include "GuiComponent.h"
#include "Box2dTransform.h"
#include "FerryShipController.h"
#include "MiningBaseController.h"

GuiComponent::GuiComponent(std::weak_ptr<Entity> parent) : Component(parent) {
	this->entityWindow = tgui::ChildWindow::create();
	this->entityTabs = tgui::Tabs::create();
	this->entityTabs->connect("TabSelected", &GuiComponent::changePanel, this);
	this->entityWindow->add(this->entityTabs);
}
void GuiComponent::update() {
	this->entityWindow->removeAllWidgets();
	this->entityWindow->setTitle(this->getParent().lock()->getStringRep());
	this->entityWindow->setPosition(this->getParent().lock()->components.transform->getPosition());
	this->entityWindow->add(this->entityTabs);
	this->entityPanels = {};
	this->entityTabs->removeAll();
	for (ComponentType c : ComponentList::allTypes) {
		if (std::shared_ptr<Component> comp = this->getParent().lock()->components.get(c)) {
			comp->updateGui(this->entityTabs, &this->entityPanels);
		}
	}
	for (auto it = this->entityPanels.begin(); it != this->entityPanels.end(); ++it) {
		//this->entityWindow->add(it->second);
		it->second->setSize({
			this->entityWindow->getFullSize().x,
			this->entityWindow->getFullSize().y - this->entityTabs->getFullSize().y});
		it->second->setPosition({ 0, this->entityTabs->getFullSize().y });
	}
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