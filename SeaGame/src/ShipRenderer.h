#pragma once
#include "RenderComponent.h"

// Render a ship onscreen by combining several sprites
// i.e. rendering the hull, then the cannon, then the sails etc
class ShipRenderer : public RenderComponent
{
public:
	// Different colors the sails can be
	const enum SailColor {Red, Blue, Green, Yellow, Black, White};
	// Convert SailColor value to string used to load the sprites
	static std::string getSailColorString(SailColor color);
	// Set angle to draw swivel gun pointing
	// Used by ShipController
	void setSwivel(float Angle);

	ShipRenderer(SailColor sailColor);
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
	// Color of sails
	SailColor sailColor;
	// Angle to draw the swivel cannon at
	float swivelAngle;
	// Set the sprites to match the damage done to the ship
	void setSprites();
	// Get the string representation of the damage done for a certain health
	std::string getDamageString();
};