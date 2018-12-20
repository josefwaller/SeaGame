#pragma once
#include "Component.h"

class ClickComponent : public Component {
public:
	virtual bool checkForClick(sf::Vector2f mousePos) = 0;
};