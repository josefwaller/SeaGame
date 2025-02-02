#include "MilitaryBaseRenderer.h"
#include "MilitaryBaseController.h"
#include "ResourceManager.h"

MilitaryBaseRenderer::MilitaryBaseRenderer() : BaseRenderer()
{
}
void MilitaryBaseRenderer::render(RenderManager& r)
{
	BaseRenderer::render(r);
	// Render all of the cannons on the base
	std::vector<Cannon> cannons = std::dynamic_pointer_cast<MilitaryBaseController>(
		this->getComponentList().controller
	)->getCannons();
	for (auto cannon : cannons) {
		// Draws a cannon in the middle
		sf::Sprite cannonSpr = ResourceManager::get()->getSprite("ships", "cannonLoose.png", true);
		cannonSpr.setScale(4, 4);
		cannonSpr.setPosition(this->getComponentList().transform->getPosition() + cannon.position);
		cannonSpr.setRotation((float)(cannon.rotation * 180 / M_PI));
		// Set origin to half height, 1/3rd width
		cannonSpr.setOrigin(sf::Vector2f(cannonSpr.getLocalBounds().width / 3, cannonSpr.getLocalBounds().height / 2));
		r.add(cannonSpr, RenderManager::INDEX_DECK);
	}
}