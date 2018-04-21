#include "MilitaryBaseRenderer.h"
#include "MilitaryBaseController.h"
#include "ResourceManager.h"

MilitaryBaseRenderer::MilitaryBaseRenderer(std::weak_ptr<Entity> parent) : BaseRenderer(parent)
{
}
void MilitaryBaseRenderer::render(RenderManager& r)
{
	BaseRenderer::render(r);
	// Render all of the cannons on the base
	std::vector<Cannon> cannons = std::dynamic_pointer_cast<MilitaryBaseController>(
		this->getParent()->controller
	)->getCannons();
	for (auto cannon : cannons) {
		// Draws a cannon in the middle
		sf::Sprite cannonSpr = ResourceManager::get()->getSprite("ships", "cannonLoose.png", true);
		cannonSpr.setScale(4, 4);
		cannonSpr.setPosition(this->getParent()->transform->getPosition() + cannon.position);
		cannonSpr.setRotation(cannon.rotation * 180 / M_PI);
		// Set origin to half height, 1/3rd width
		cannonSpr.setOrigin(sf::Vector2f(cannonSpr.getLocalBounds().width / 3, cannonSpr.getLocalBounds().height / 2));
		r.add(cannonSpr, RenderManager::INDEX_DECK);
	}
}