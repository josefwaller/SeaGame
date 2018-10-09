#pragma once
#include <memory>
#include "Component.h"

// Forward declaration of components
class RenderComponent;
class TransformComponent;
class HealthComponent;
class ControllerComponent;
class PhysicsComponent;
class InventoryComponent;
class GuiComponent;
class ClickComponent;


// Hold the components for an entity. Is iterable and accessable
enum ComponentType {
	Transform,
	Renderer,
	Physics,
	Controller,
	Health,
	Inventory,
	Gui,
	Click
};
struct ComponentList {
	// The components
	std::shared_ptr<TransformComponent> transform;
	std::shared_ptr<RenderComponent> renderer;
	std::shared_ptr<PhysicsComponent> physics;
	std::shared_ptr<ControllerComponent> controller;
	std::shared_ptr<HealthComponent> health;
	std::shared_ptr<InventoryComponent> inventory;
	std::shared_ptr<GuiComponent> gui;
	std::shared_ptr<ClickComponent> click;
	// Get the component 
	std::shared_ptr<Component> get(ComponentType t) {
		switch (t) {
		case ComponentType::Controller: return std::dynamic_pointer_cast<Component>(controller);
		case ComponentType::Renderer: return std::dynamic_pointer_cast<Component>(renderer);
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