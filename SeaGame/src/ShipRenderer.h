#pragma once
#include "RenderComponent.h"

// Render a ship onscreen by combining several sprites
// i.e. rendering the hull, then the cannon, then the sails etc
class ShipRenderer : public RenderComponent
{
public:
	// Different colors the sails can be
	const enum SAIL_COLOR {Red, Blue, Green, Yellow, Black, White};
	// Convert SAIL_COLOR value to string used to load the sprites
	static std::string getSailColorString(SAIL_COLOR color);

	ShipRenderer(std::weak_ptr<Entity> e, SAIL_COLOR sailColor);
	virtual void render(RenderManager& r) override;
	virtual void reset() override;
private:
	// The sprites for various parts of the ships
	sf::Sprite hull;
	sf::Sprite bigSail;
	sf::Sprite smallSail;
	sf::Sprite swivelCannon;
	// Some ships do not have small sails
	bool hasSmallSail;
	// Angle to draw the swivel cannon at
	float swivelAngle;
};