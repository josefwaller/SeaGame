#include "CityRenderer.h"
#include "ResourceManager.h"

CityRenderer::CityRenderer(std::weak_ptr<Entity> parent) : BaseRenderer(parent) {
	// Add house
	// ToDo: make there more houses
	this->houses = {
		ResourceManager::get()->getSprite("medievalRTS_spritesheet@2", "medievalStructure_20.png", true)
	};
	this->houses[0].setPosition(this->getParent().lock()->components.transform->getPosition() + sf::Vector2f(64.0f * 1.5, 64.0f * 1.5));
	this->houses[0].setScale({ 0.7f, 0.7f });
}

void CityRenderer::render(RenderManager& rm) {
	BaseRenderer::render(rm);
	rm.add(this->houses[0], RenderManager::INDEX_LAND_TILES);
}