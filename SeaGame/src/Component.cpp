#include "Component.h"

Component::Component(std::weak_ptr<Entity> newParent): parent(newParent)
{
}
std::weak_ptr<Entity> Component::getParent()
{
	return this->parent;
}