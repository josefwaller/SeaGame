#include "MiningBaseRenderer.h"
#include "ResourceManager.h"

MiningBaseRenderer::MiningBaseRenderer(std::weak_ptr<Entity> parent, GameResource res) : BaseRenderer(parent) {
	this->resSprite = getResourceSprite(res);
	this->resSprite.setPosition(this->getParent()->transform->getPosition() + sf::Vector2f(64.0f * 1.5, 64.0f * 1.5));
}
void MiningBaseRenderer::render(RenderManager& rm) {
	BaseRenderer::render(rm);
	rm.add(this->resSprite, RenderManager::INDEX_BASE_TILES);
}
sf::Sprite MiningBaseRenderer::getResourceSprite(GameResource res) {
	return ResourceManager::get()->getSprite("medievalRTS_spritesheet@2", "medievalEnvironment_11.png", true);
}