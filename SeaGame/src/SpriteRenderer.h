#pragma once
#include "RenderComponent.h"

class SpriteRenderer : public RenderComponent
{
public:
	SpriteRenderer(std::weak_ptr<Entity> parent, std::string sheetName, std::string spriteName);
	virtual void render(sf::RenderWindow& w) override;

private:
	sf::Sprite spr;
};