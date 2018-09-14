#pragma once
#include "BaseRenderer.h"

class CityRenderer : public BaseRenderer {
public:
	CityRenderer(std::weak_ptr<Entity> parent);
	virtual void render(RenderManager& rm) override;
private:
	std::vector<sf::Sprite> houses;
};