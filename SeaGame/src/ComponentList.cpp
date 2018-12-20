#include "ComponentList.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "ClickComponent.h"

void ComponentList::set(std::weak_ptr<Entity> parent) {
	for (ComponentType t : allTypes) {
		if (this->get(t)) {
			this->get(t)->setParent(parent);
		}
	}
}

std::vector<ComponentType> ComponentList::allTypes = {
	ComponentType::Transform,
	ComponentType::Renderer,
	ComponentType::Physics,
	ComponentType::Controller,
	ComponentType::Health,
	ComponentType::Inventory,
	ComponentType::Gui,
	ComponentType::Click
};
