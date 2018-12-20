#include "Component.h"
#include "Game.h"

Component::Component()
{
}
std::weak_ptr<Entity> Component::getParent()
{
	return this->parent;
}
void Component::setParent(std::weak_ptr<Entity> parent) {
	this->parent = parent;
}
std::map<std::string, std::string> Component::getSaveData() {
	return {};
}

void Component::fromSaveData(std::map<std::string, std::string> data) {
}

void Component::updateGui(tgui::Tabs::Ptr tabs, std::map<std::string, tgui::Panel::Ptr>* panels) {}