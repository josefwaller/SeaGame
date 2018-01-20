#include "SpriteRenderer.h"
#include "ResourceManager.h"

SpriteRenderer::SpriteRenderer(std::weak_ptr<Entity> parent, std::string sheetName, std::string spriteName) : RenderComponent(parent)
{
	this->spr = ResourceManager::get()->getSprite(sheetName, spriteName, true);
}
void SpriteRenderer::render(sf::RenderWindow& w)
{
	if (auto transform = this->getParent()->transform)
	{
		this->spr.setPosition(transform->getPosition());
		// Convert to degrees because dumbass SFML uses degrees
		this->spr.setRotation(transform->getRotation() / M_PI * 180.0f);
	}
	w.draw(this->spr);
}