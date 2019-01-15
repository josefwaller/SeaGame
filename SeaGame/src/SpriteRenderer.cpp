#include "SpriteRenderer.h"
#include "ResourceManager.h"

SpriteRenderer::SpriteRenderer(
	std::string sheetName,
	std::string spriteName,
	size_t zIndex)
{
	this->spr = ResourceManager::get()->getSprite(sheetName, spriteName, true);
	this->zIndex = zIndex;
}
SpriteRenderer::SpriteRenderer(sf::Sprite spr, size_t zIndex) : RenderComponent() {
	this->spr = spr;
	this->zIndex = zIndex;
}
void SpriteRenderer::render(RenderManager& r)
{
	if (auto transform = this->getParent().lock()->components.transform)
	{
		this->spr.setPosition(transform->getPosition());
		// Convert to degrees because dumbass SFML uses degrees
		this->spr.setRotation(transform->getRotation() / (float)M_PI * 180.0f);
	}
	r.add(this->spr, this->zIndex);
}