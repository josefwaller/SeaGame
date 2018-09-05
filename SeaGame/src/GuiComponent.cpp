#include "GuiComponent.h"
#include "Box2dTransform.h"

GuiComponent::GuiComponent(std::shared_ptr<Entity> parent) : Component(parent) {
	this->entityWindow = tgui::ChildWindow::create();
	this->entityTabs = tgui::Tabs::create();
	this->entityTabs->add("Inventory");
	this->entityTabs->add("Health");
	this->entityTabs->select("Inventory");
	this->entityTabs->connect("TabSelected", &GuiComponent::changePanel, this);
	this->selectedPanel = "Inventory";
	this->entityPanels["Inventory"] = tgui::Panel::create();
	this->entityPanels["Health"] = tgui::Panel::create();
	this->entityPanels["Inventory"]->setPosition({ 0, this->entityTabs->getFullSize().y });
	this->updateInventory();
	this->entityWindow->add(this->entityTabs);
	this->entityWindow->add(this->entityPanels["Inventory"]);
}

void GuiComponent::changePanel(std::string selectedPanel) {
	this->entityWindow->remove(this->entityPanels[this->selectedPanel]);
	this->selectedPanel = selectedPanel;
	this->entityWindow->add(this->entityPanels[this->selectedPanel]);
	this->entityPanels[this->selectedPanel]->setPosition({ 0, this->entityTabs->getFullSize().y });
}
void GuiComponent::checkForClick(sf::Vector2f mouseCoords) {
	if (auto trans = std::dynamic_pointer_cast<Box2dTransform>(this->getParent()->transform)) {
		auto body = trans->getBody();
		for (auto fix = body->GetFixtureList(); fix; fix = fix->GetNext()) {
			if (fix->TestPoint(b2Vec2(mouseCoords.x, mouseCoords.y))) {
				this->onClick();
				return;
			}
		}

	}
}
std::string GuiComponent::getResourceString(GameResource res) {
	switch (res) {
	case GameResource::Gold: return "Gold";
	case GameResource::Wood: return "Wood";
	case GameResource::Stone: return "Stone";
	}
	return "N/A";
}


void GuiComponent::updateInventory() {
	this->entityPanels["Inventory"]->removeAllWidgets();
	auto list = tgui::ListBox::create();
	if (this->getParent()->inventory != nullptr) {
		for (auto it : this->getParent()->inventory->getInventory()) {
			list->addItem(this->getResourceString(it.first) + ": " + std::to_string(it.second));
		}
	}
	this->entityPanels["Inventory"]->add(list);
}

void GuiComponent::onClick() {
	if (!this->entityWindow->getParent()) {
		this->getParent()->game->getGui().add(this->entityWindow);
	}
}