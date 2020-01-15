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

	ComponentList();
	ComponentList(
		TransformComponent* t,
		ControllerComponent* c,
		RenderComponent* r,
		PhysicsComponent* p,
		HealthComponent* h,
		InventoryComponent* i,
		GuiComponent* g,
		ClickComponent* cl);
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
	std::shared_ptr<Component> get(ComponentType t);
};