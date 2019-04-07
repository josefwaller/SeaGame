#pragma once
#include "ControllerComponent.h"
#include <SFML/System/Vector2.hpp>

// Basically only defined dock coords, i.e. where ships go to
// deliver/take resources
// Must be on water
class BaseController : public ControllerComponent {
public:
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void onDeath() override;
	sf::Vector2f getDockCoords();
private:
	sf::Vector2f dockCoords;
};