#include "ShipRenderer.h"
#include "ResourceManager.h"

std::string ShipRenderer::getSailColorString(ShipRenderer::SAIL_COLOR color)
{
	switch (color) {
	case SAIL_COLOR::Red:
		return "Red";
	case SAIL_COLOR::Blue:
		return "Blue";
	case SAIL_COLOR::Yellow:
		return "Yellow";
	case SAIL_COLOR::Green:
		return "Green";
	case SAIL_COLOR::Black:
		return "Black";
	case SAIL_COLOR::White:
		return "White";
	default:
		return "White";
	}
}
ShipRenderer::ShipRenderer(std::weak_ptr<Entity> parent, SAIL_COLOR sailColor) : RenderComponent(parent)
{
	this->hull = ResourceManager::get()->getSprite("ships", "hullLarge (1).png", true);
	this->bigSail = ResourceManager::get()->getSprite(
		"ships",
		"sailLarge" + getSailColorString(sailColor) + ".png",
		true);
	this->hasSmallSail = (sailColor != SAIL_COLOR::Black && sailColor != SAIL_COLOR::White);
	if (this->hasSmallSail) {
		this->smallSail = ResourceManager::get()->getSprite(
			"ships",
			"sailSmall" + getSailColorString(sailColor) + ".png",
			true);
	}
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
	if (this->hasSmallSail)
		positionLayoutSprite(this->smallSail, layout["smallSail"], pos, rot);
	positionLayoutSprite(this->swivelCannon, layout["swivelCannon"], pos, cont->getSwivelAngle() * 180.0f / M_PI);
	// Draw in proper order
	r.add(this->hull, RenderManager::INDEX_HULL);
	r.add(this->swivelCannon, RenderManager::INDEX_DECK);
	r.add(this->bigSail, RenderManager::INDEX_SAILS);
	if (this->hasSmallSail)
		r.add(this->smallSail, RenderManager::INDEX_SAILS);

}