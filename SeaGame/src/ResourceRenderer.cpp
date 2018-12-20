#include "ResourceRenderer.h"

ResourceRenderer::ResourceRenderer(GameResource res) {
	this->spr = getResourceSprite(res, true);
}
void ResourceRenderer::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
#ifdef _DEBUG
	if (!parent.lock()->components.transform) {
		throw std::runtime_error("Cannot add a resource render without first setting up a transform component");
	}
#endif
	sf::Vector2f pos = parent.lock()->components.transform->getPosition() + sf::Vector2f(32.0f, 32.0f);
	this->spr.setPosition(pos);
}
void ResourceRenderer::render(RenderManager& rm) {
	rm.add(this->spr, RenderManager::INDEX_HULL);
}