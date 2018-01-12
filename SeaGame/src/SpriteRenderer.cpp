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
	w.draw(this->spr);
}