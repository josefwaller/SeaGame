#pragma once
#include "ControllerComponent.h"

// Basically only defined dock coords, i.e. where ships go to
// deliver/take resources
// Must be on water
class BaseController : public ControllerComponent {
public:
	BaseController();
	BaseController(std::weak_ptr<Entity> parent);
	sf::Vector2f getDockCoords();
private:
	sf::Vector2f dockCoords;
};