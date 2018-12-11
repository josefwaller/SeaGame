#pragma once
#include "RenderComponent.h"

class SpriteRenderer : public RenderComponent
{
public:
	SpriteRenderer(std::weak_ptr<Entity> parent, std::string sheetName, std::string spriteName, size_t zIndex = 0);
	SpriteRenderer(std::weak_ptr<Entity> parent, sf::Sprite spr, size_t zIndex = 0);
	virtual void render(RenderManager& r) override;

private:
	sf::Sprite spr;
	// z index to draw the sprite at
	size_t zIndex;
};