#pragma once
#include "ClickComponent.h"

class RectangleClick : public ClickComponent {
public:
	RectangleClick(sf::Vector2i offset, sf::Vector2i size);
	virtual bool checkForClick(sf::Vector2f mouseCoords) override;
private:
	sf::FloatRect rect;
};
