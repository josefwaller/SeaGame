#pragma once
#include "RenderComponent.h"

class BaseRenderer : public RenderComponent
{
public:
	BaseRenderer();
	virtual void render(RenderManager& r) override;
private:
	sf::Sprite tower;
	sf::Sprite vertWall;
	sf::Sprite horizWall;
protected:
	// Draw a vertical or horizontal wall to a point
	void renderHorizontalWall(RenderManager& r, sf::Vector2f start, int length);
	void renderVerticalWall(RenderManager& r, sf::Vector2f start, int length);
};
