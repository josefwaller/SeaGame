#pragma once
#include "ClickComponent.h"

class Box2dClick : public ClickComponent {
public:
	Box2dClick();
	virtual bool checkForClick(sf::Vector2f mousePos) override;
};