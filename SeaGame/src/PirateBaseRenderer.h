#pragma once
#include "BaseRenderer.h"

class PirateBaseRenderer : public BaseRenderer {
public:
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void render(RenderManager& r) override;
private:
	// The sprites of the ship to draw at the middle of the base
	std::vector<sf::Sprite> shipSprites;
};