#pragma once
#include "Component.h"

class ClickComponent : public Component {
public:
	ClickComponent();
	ClickComponent(std::weak_ptr<Entity> parent);
	virtual bool checkForClick(sf::Vector2f mousePos) = 0;
};