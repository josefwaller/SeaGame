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
sf::Sprite MiningBaseRenderer::getResourceSprite(GameResource res) {
	std::string spriteName;
	switch (res) {
	case GameResource::Stone:
		spriteName = "medievalEnvironment_08.png";
		break;
	case GameResource::Gold:
		spriteName = "medievalEnvironment_11.png";
		break;
	case GameResource::Wood:
		spriteName = "medievalEnvironment_03.png";
		break;
	}
	return ResourceManager::get()->getSprite("medievalRTS_spritesheet@2", spriteName, true);
}