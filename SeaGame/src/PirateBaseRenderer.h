#pragma once
#include "BaseRenderer.h"

class PirateBaseRenderer : public BaseRenderer {
public:
	PirateBaseRenderer(std::weak_ptr<Entity> parent);
	virtual void render(RenderManager& r) override;
private:
	// The sprites of the ship to draw at the middle of the base
	std::vector<sf::Sprite> shipSprites;
};