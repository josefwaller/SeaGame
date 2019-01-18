#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>

#define ALL_COMPONENT_TYPES \
	etype(Transform) \
	etype(Render) \
	etype(Physics) \
	etype(Controller) \
	etype(Health) \
	etype(Inventory) \
	etype(Gui) \
	etype(Click)

// Forward declaration of components
class Component;
class RenderComponent;
class TransformComponent;
class HealthComponent;
class ControllerComponent;
class PhysicsComponent;
class InventoryComponent;
class GuiComponent;
class ClickComponent;
// Forward declaration of entity
struct Entity;

// Hold the components for an entity. Is iterable and accessable
#define etype(x) x,
enum ComponentType {
	ALL_COMPONENT_TYPES
};
#undef etype
struct ComponentList {
	static std::map<ComponentType, std::string> ComponentStrings;

	ComponentList() {}
	ComponentList(
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
	// The components
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
	std::shared_ptr<PhysicsComponent> physics;
	std::shared_ptr<ControllerComponent> controller;
	std::shared_ptr<HealthComponent> health;
	std::shared_ptr<InventoryComponent> inventory;
	std::shared_ptr<GuiComponent> gui;
	std::shared_ptr<ClickComponent> click;
	// Vector of all the types of components
	static std::vector<ComponentType> allTypes;
	// "Set" the components - call their set method to set their parent
	// See Component::set(std::shared_ptr<Entity> parent)
	void set(std::weak_ptr<Entity> parent);
	// Get the component 
	std::shared_ptr<Component> get(ComponentType t) {
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
};

#include "Component.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ControllerComponent.h"
#include "PhysicsComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "ClickComponent.h"


