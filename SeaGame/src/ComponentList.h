#pragma once
#include <memory>

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
	template<class T>
	std::shared_ptr<T> get(ComponentType t) {
		if (!std::is_base_of<Component, T>()) {
			auto m = "idiot";
			auto x = 0;
		}
		switch (t) {
		case ComponentType::Controller: return std::dynamic_pointer_cast<T>(controller);
		case ComponentType::Renderer: return std::dynamic_pointer_cast<T>(renderer);
		case ComponentType::Transform: return std::dynamic_pointer_cast<T>(transform);
		case ComponentType::Physics: return std::dynamic_pointer_cast<T>(physics);
		case ComponentType::Health: return std::dynamic_pointer_cast<T>(health);
		case ComponentType::Inventory: return std::dynamic_pointer_cast<T>(inventory);
		case ComponentType::Gui: return std::dynamic_pointer_cast<T>(gui);
		case ComponentType::Click: return std::dynamic_pointer_cast<T>(click);
		}
	}
};

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "ControllerComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "ClickComponent.h"