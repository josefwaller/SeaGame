#include "Component.h"

Component::Component(std::weak_ptr<Entity> newParent): parent(newParent)
{
}
std::shared_ptr<Entity> Component::getParent()
{
	return this->parent.lock();
}