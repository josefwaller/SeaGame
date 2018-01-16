#include "SpriteRenderer.h"
#include "ResourceManager.h"

SpriteRenderer::SpriteRenderer(Entity& parent, std::string sheetName, std::string spriteName) : RenderComponent(parent)
{
	this->spr = sf::Sprite();
	auto spriteInfo = ResourceManager::get()->getSpriteInfo(sheetName, spriteName);
	this->spr.setTexture(*spriteInfo.first);
	this->spr.setTextureRect(spriteInfo.second);
}
void SpriteRenderer::render(sf::RenderWindow& w)
{
	if (auto transform = this->getParent().transform)
	{
		this->spr.setPosition(transform->getPosition());
		// Convert to degrees because dumbass SFML uses degrees
		this->spr.setRotation(transform->getRotation() / M_PI * 180.0f);
	}
	w.draw(this->spr);
}