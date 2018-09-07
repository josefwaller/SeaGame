#pragma once
#include "ClickComponent.h"

class Box2dClick : public ClickComponent {
public:
	Box2dClick();
	Box2dClick(std::weak_ptr<Entity> parent);
	virtual bool checkForClick(sf::Vector2f mousePos) override;
};