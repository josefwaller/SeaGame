#pragma once
#include "BaseRenderer.h"

class CityRenderer : public BaseRenderer {
public:
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void render(RenderManager& rm) override;
	virtual void reset() override;
private:
	std::vector<sf::Sprite> houses;
};