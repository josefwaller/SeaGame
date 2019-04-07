#include "Component.h"
#include "ComponentList.h"
#include "SaveData.h"
#include "Game.h"
#include "Entity.h"

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
ComponentList Component::getComponentList() {
	return this->getParent().lock()->components;
}
Game* Component::getGame() {
	return this->parent.lock()->game;
}
tgui::Widget::Ptr Component::getGui() {
	return tgui::Widget::Ptr();
}
