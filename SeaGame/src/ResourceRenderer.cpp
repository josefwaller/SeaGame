#include "ResourceRenderer.h"
#include "ResourceController.h"

ResourceRenderer::ResourceRenderer(GameResource res) {
}
void ResourceRenderer::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
#ifdef _DEBUG
	if (!parent.lock()->components.transform) {
		throw std::runtime_error("Cannot add a resource render without first setting up a transform component");
	}
#endif
	this->reset();
}
void ResourceRenderer::render(RenderManager& rm) {
	rm.add(this->spr, RenderManager::INDEX_HULL);
}
void ResourceRenderer::reset() {
	// Get the resource from the controller
	GameResource res = std::dynamic_pointer_cast<ResourceController>(this->getParent().lock()->components.controller)->getResource();
	this->spr = getResourceSprite(res, true);
	// Set the position
	sf::Vector2f pos = this->getParent().lock()->components.transform->getPosition() + sf::Vector2f(32.0f, 32.0f);
	this->spr.setPosition(pos);
}