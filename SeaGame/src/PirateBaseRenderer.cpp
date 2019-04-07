#include "PirateBaseRenderer.h"
#include "ResourceManager.h"

void PirateBaseRenderer::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	this->shipSprites = {
		ResourceManager::get()->getSprite("ships", "hullLarge.png", true),
		ResourceManager::get()->getSprite("ships", "sailLargeBlack.png", true)
	};
	for (auto it = this->shipSprites.begin(); it != this->shipSprites.end(); it++) {
		it->setPosition(this->getComponentList().transform->getPosition() + sf::Vector2f(64.0f * 1.5, 64.0f * 1.5));
	}
}

void PirateBaseRenderer::render(RenderManager& r) {
	BaseRenderer::render(r);
	for (auto s : this->shipSprites) {
		r.add(s, RenderManager::INDEX_DECK);
	}
}