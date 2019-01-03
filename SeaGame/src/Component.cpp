#include "Component.h"
#include "Game.h"

Component::Component()
{
}
Component::~Component() {}
std::weak_ptr<Entity> Component::getParent()
{
	return this->parent;
}
void Component::setParent(std::weak_ptr<Entity> parent) {
	this->parent = parent;
}
SaveData Component::getSaveData() {
	return SaveData("Component");
}
void Component::fromSaveData(SaveData data) {
}

void Component::updateGui(tgui::Tabs::Ptr tabs, std::map<std::string, tgui::Panel::Ptr>* panels) {}