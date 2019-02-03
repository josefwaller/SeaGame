#include "Component.h"
#include "SaveData.h"
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
Game* Component::getGame() {
	return this->parent.lock()->game;
}
tgui::Widget::Ptr Component::getGui() {
	return tgui::Widget::Ptr();
}
