#include "Component.h"

Component::Component(std::weak_ptr<Entity> newParent): parent(newParent)
{
}
std::weak_ptr<Entity> Component::getParent()
{
	return this->parent;
}
std::map<std::string, std::string> Component::getSaveData() {
	return {};
}