#pragma once
#include "RenderComponent.h"

// Render a ship onscreen by combining several sprites
// i.e. rendering the hull, then the cannon, then the sails etc
class ShipRenderer : public RenderComponent
{
public:
	ShipRenderer(Entity& e, std::string sailColor);
	virtual void render(sf::RenderWindow& w) override;
private:
	// The sprites for various parts of the ships
	sf::Sprite hull;
	sf::Sprite bigSail;
	sf::Sprite smallSail;
};