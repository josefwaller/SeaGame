#include "ShipRenderer.h"
#include "ResourceManager.h"
#include "ShipController.h"

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
	this->hull = ResourceManager::get()->getSprite("ships", "hullLarge.png", true);
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
	this->sailColor = sailColor;
}
void ShipRenderer::setSwivel(float angle)
{
	this->swivelAngle = angle;
}

void ShipRenderer::render(RenderManager& r)
{
	// Get the layout
	std::map<std::string, LayoutSprite> layout = ResourceManager::get()->getLayout("smallShip");
	// Get this entity's position and rotation for easy access
	sf::Vector2f pos = this->getParent()->transform->getPosition();
	float rot = this->getParent()->transform->getRotation() * 180.0f / M_PI;
	// Position all sprites in the ship
	positionLayoutSprite(this->hull, layout["hull"], pos, rot);
	positionLayoutSprite(this->bigSail, layout["bigSail"], pos, rot);
	if (this->hasSmallSail)
		positionLayoutSprite(this->smallSail, layout["smallSail"], pos, rot);
	positionLayoutSprite(this->swivelCannon, layout["swivelCannon"], pos, this->swivelAngle * 180.0f / M_PI);
	// Draw in proper order
	r.add(this->hull, RenderManager::INDEX_HULL);
	r.add(this->swivelCannon, RenderManager::INDEX_DECK);
	r.add(this->bigSail, RenderManager::INDEX_SAILS);
	if (this->hasSmallSail)
		r.add(this->smallSail, RenderManager::INDEX_SAILS);

}
void ShipRenderer::reset()
{
	if (auto cont = std::dynamic_pointer_cast<ShipController>(this->getParent()->controller)) {
		this->swivelAngle = cont->getSwivelAngle();
	}
	this->setSprites();
}

void ShipRenderer::setSprites()
{
	int health;
	if (this->getParent()->health != nullptr)
		health = this->getParent()->health->getHealth(HealthType::Default);
	else
		health = 100;

	std::string damageString = "";
	if (health == 0)
		damageString = "Destroyed";

	this->bigSail = ResourceManager::get()->getSprite(
		"ships",
		"sailLarge" + this->getSailColorString(this->sailColor) + damageString + ".png",
		true);
	if (this->hasSmallSail) {
		// Small sails only have 1 destroyed sprite, rather than 1 for each color
		// So if the small sail is destroyed, don't have a color string
		std::string colorName = this->getSailColorString(this->sailColor);
		if (health == 0)
			colorName = "";
		this->smallSail = ResourceManager::get()->getSprite(
			"ships",
			"sailSmall" + colorName + damageString + ".png",
			true
		);
	}
	this->hull = ResourceManager::get()->getSprite(
		"ships",
		"hullLarge" + damageString + ".png",
		true
	);
}