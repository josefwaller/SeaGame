#include "Component.h"

Component::Component(Entity& newParent): parent(newParent)
{
}
Entity& Component::getParent()
{
	return this->parent;
}