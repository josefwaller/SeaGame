#include "MiningBaseRenderer.h"
#include "ResourceManager.h"

MiningBaseRenderer::MiningBaseRenderer(GameResource res) {
	this->resSprite = getResourceSprite(res);
}
void MiningBaseRenderer::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	this->resSprite.setPosition(this->getParent().lock()->components.transform->getPosition() + sf::Vector2f(64.0f * 1.5, 64.0f * 1.5));
}
void MiningBaseRenderer::render(RenderManager& rm) {
	BaseRenderer::render(rm);
	rm.add(this->resSprite, RenderManager::INDEX_BASE_TILES);
}