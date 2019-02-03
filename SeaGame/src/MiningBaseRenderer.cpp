#include "MiningBaseRenderer.h"
#include "ResourceManager.h"
#include "SaveData.h"

MiningBaseRenderer::MiningBaseRenderer(GameResource res) {
	this->resSprite = getResourceSprite(res);
	this->res = res;
}
void MiningBaseRenderer::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	this->reset();
}
void MiningBaseRenderer::render(RenderManager& rm) {
	BaseRenderer::render(rm);
	rm.add(this->resSprite, RenderManager::INDEX_BASE_TILES);
}
void MiningBaseRenderer::reset() {
	this->resSprite.setPosition(this->getParent().lock()->components.transform->getPosition() + sf::Vector2f(64.0f * 1.5, 64.0f * 1.5));
}
SaveData MiningBaseRenderer::getSaveData() {
	return SaveData("Component", {
		{ "res", std::to_string(this->res) }
	});
}
void MiningBaseRenderer::fromSaveData(SaveData sd) {
	this->res = (GameResource)std::stoi(sd.getValue("res"));
	this->resSprite = getResourceSprite((GameResource)std::stoi(sd.getValue("res")));
}