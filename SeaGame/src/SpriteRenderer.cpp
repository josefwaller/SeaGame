#include "SpriteRenderer.h"
#include "ResourceManager.h"

SpriteRenderer::SpriteRenderer(Entity& parent, std::string sheetName, std::string spriteName) : RenderComponent(parent)
{
	this->spr = ResourceManager::get()->getSprite(sheetName, spriteName);
}
void SpriteRenderer::render(sf::RenderWindow& w)
{
	w.draw(this->spr);
}