#include "ShipRenderer.h"
#include "ResourceManager.h"

ShipRenderer::ShipRenderer(Entity& parent, std::string sailColor) : RenderComponent(parent)
{
	this->hull = ResourceManager::get()->getSprite("ships", "hullLarge (1).png", true);
	this->bigSail = ResourceManager::get()->getSprite("ships", "sailLargeRed.png", true);
	this->smallSail = ResourceManager::get()->getSprite("ships", "sailSmallRed.png", true);
}

void ShipRenderer::render(sf::RenderWindow& w)
{
	// Get the layout
	std::map<std::string, LayoutSprite> layout = ResourceManager::get()->getLayout("smallShip");
	// Get this entity's position and rotation for easy access
	sf::Vector2f pos = this->getParent().transform->getPosition();
	float rot = this->getParent().transform->getRotation() * 180.0f / M_PI;
	// Position all sprites in the ship
	positionLayoutSprite(this->hull, layout["hull"], pos, rot);
	positionLayoutSprite(this->bigSail, layout["bigSail"], pos, rot);
	positionLayoutSprite(this->smallSail, layout["smallSail"], pos, rot);
	// Draw in proper order
	w.draw(this->hull);
	w.draw(this->bigSail);
	w.draw(this->smallSail);
}