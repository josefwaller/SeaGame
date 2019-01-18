#include "ComponentList.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "ClickComponent.h"

// String representations of the different types of components for saving
#define etype(x) { ComponentType::x, #x },
std::map<ComponentType, std::string> ComponentList::ComponentStrings = {
	ALL_COMPONENT_TYPES
};
#undef etype


void ComponentList::set(std::weak_ptr<Entity> parent) {
	for (ComponentType t : allTypes) {
		if (this->get(t)) {
			this->get(t)->setParent(parent);
		}
	}
}

#define etype(x) ComponentType::x,
std::vector<ComponentType> ComponentList::allTypes = {
	ALL_COMPONENT_TYPES
};
#undef etype
#undef ALL_COMPONENT_TYPES
