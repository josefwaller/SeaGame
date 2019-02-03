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

ComponentList::ComponentList() {}
ComponentList::ComponentList(
	TransformComponent* t,
	ControllerComponent* c,
	RenderComponent* r,
	PhysicsComponent* p,
	HealthComponent* h,
	InventoryComponent* i,
	GuiComponent* g,
	ClickComponent* cl)
		: transform(t),
		renderer(r),
		physics(p),
		controller(c),
		health(h),
		inventory(i),
		gui(g),
		click(cl) {}

void ComponentList::set(std::weak_ptr<Entity> parent) {
	for (ComponentType t : allTypes) {
		if (this->get(t)) {
			this->get(t)->setParent(parent);
		}
	}
}
std::shared_ptr<Component> ComponentList::get(ComponentType t) {
	switch (t) {
	case ComponentType::Controller: return std::dynamic_pointer_cast<Component>(controller);
	case ComponentType::Render: return std::dynamic_pointer_cast<Component>(renderer);
	case ComponentType::Transform: return std::dynamic_pointer_cast<Component>(transform);
	case ComponentType::Physics: return std::dynamic_pointer_cast<Component>(physics);
	case ComponentType::Health: return std::dynamic_pointer_cast<Component>(health);
	case ComponentType::Inventory: return std::dynamic_pointer_cast<Component>(inventory);
	case ComponentType::Gui: return std::dynamic_pointer_cast<Component>(gui);
	case ComponentType::Click: return std::dynamic_pointer_cast<Component>(click);
	default: return nullptr;
	}
}

#define etype(x) ComponentType::x,
std::vector<ComponentType> ComponentList::allTypes = {
	ALL_COMPONENT_TYPES
};
#undef etype
#undef ALL_COMPONENT_TYPES
