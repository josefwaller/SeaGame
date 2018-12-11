#include "MiningBaseRenderer.h"
#include "ResourceManager.h"

MiningBaseRenderer::MiningBaseRenderer(std::weak_ptr<Entity> parent, GameResource res) : BaseRenderer(parent) {
	this->resSprite = getResourceSprite(res);
	this->resSprite.setPosition(this->getParent().lock()->components.transform->getPosition() + sf::Vector2f(64.0f * 1.5, 64.0f * 1.5));
}
void MiningBaseRenderer::render(RenderManager& rm) {
	BaseRenderer::render(rm);
	rm.add(this->resSprite, RenderManager::INDEX_BASE_TILES);
}