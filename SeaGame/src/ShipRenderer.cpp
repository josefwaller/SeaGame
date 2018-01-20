#include "ShipRenderer.h"
#include "ResourceManager.h"

ShipRenderer::ShipRenderer(std::weak_ptr<Entity> parent, std::string sailColor) : RenderComponent(parent)
{
	this->hull = ResourceManager::get()->getSprite("ships", "hullLarge (1).png", true);
	this->bigSail = ResourceManager::get()->getSprite("ships", "sailLargeBlue.png", true);
	this->smallSail = ResourceManager::get()->getSprite("ships", "sailSmallBlue.png", true);
	this->swivelCannon = ResourceManager::get()->getSprite("ships", "cannonLoose.png", true);
	this->swivelCannon.setOrigin(sf::Vector2f(
		this->swivelCannon.getLocalBounds().width / 4,
		this->swivelCannon.getLocalBounds().height / 2
	));
	this->swivelCannon.setScale(2.0f, 2.0f);

}

void ShipRenderer::render(RenderManager& r)
{
	// Get the layout
	std::map<std::string, LayoutSprite> layout = ResourceManager::get()->getLayout("smallShip");
	auto cont = std::dynamic_pointer_cast<ShipController>(this->getParent()->controller);
	// Get this entity's position and rotation for easy access
	sf::Vector2f pos = this->getParent()->transform->getPosition();
	float rot = this->getParent()->transform->getRotation() * 180.0f / M_PI;
	// Position all sprites in the ship
	positionLayoutSprite(this->hull, layout["hull"], pos, rot);
	positionLayoutSprite(this->bigSail, layout["bigSail"], pos, rot);
	positionLayoutSprite(this->smallSail, layout["smallSail"], pos, rot);
	positionLayoutSprite(this->swivelCannon, layout["swivelCannon"], pos, cont->getSwivelAngle() * 180.0f / M_PI);
	// Draw in proper order
	r.addSprite(this->hull, RenderManager::INDEX_HULL);
	r.addSprite(this->swivelCannon, RenderManager::INDEX_DECK);
	r.addSprite(this->bigSail, RenderManager::INDEX_SAILS);
	r.addSprite(this->smallSail, RenderManager::INDEX_SAILS);
}