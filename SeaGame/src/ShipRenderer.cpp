#include "ShipRenderer.h"
#include "ResourceManager.h"
#include "ShipController.h"
#include "AutomatedShipController.h"

std::string ShipRenderer::getSailColorString(ShipRenderer::SailColor color)
{
	switch (color) {
	case SailColor::Red:
		return "Red";
	case SailColor::Blue:
		return "Blue";
	case SailColor::Yellow:
		return "Yellow";
	case SailColor::Green:
		return "Green";
	case SailColor::Black:
		return "Black";
	case SailColor::White:
		return "White";
	default:
		return "White";
	}
}
ShipRenderer::ShipRenderer(SailColor sailColor)
{
	this->hull = ResourceManager::get()->getSprite("ships", "hullLarge.png", true);
	this->bigSail = ResourceManager::get()->getSprite(
		"ships",
		"sailLarge" + getSailColorString(sailColor) + ".png",
		true);
	this->hasSmallSail = (sailColor != SailColor::Black && sailColor != SailColor::White);
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
	sf::Vector2f pos = this->getParent().lock()->components.transform->getPosition();
	float rot = this->getParent().lock()->components.transform->getRotation() * 180.0f / (float)M_PI;
	// Position all sprites in the ship
	positionLayoutSprite(this->hull, layout["hull"], pos, rot);
	positionLayoutSprite(this->bigSail, layout["bigSail"], pos, rot);
	if (this->hasSmallSail)
		positionLayoutSprite(this->smallSail, layout["smallSail"], pos, rot);
	positionLayoutSprite(this->swivelCannon, layout["swivelCannon"], pos, this->swivelAngle * 180.0f / (float)M_PI);
	// Draw in proper order
	r.add(this->hull, RenderManager::INDEX_HULL);
	r.add(this->swivelCannon, RenderManager::INDEX_DECK);
	r.add(this->bigSail, RenderManager::INDEX_SAILS);
	if (this->hasSmallSail)
		r.add(this->smallSail, RenderManager::INDEX_SAILS);
	if (auto c = std::dynamic_pointer_cast<AutomatedShipController>(this->getParent().lock()->components.controller)) {
		sf::Sprite s = ResourceManager::get()->getSprite("ships", "crew (1).png", true);
		for (auto it : c->points) {
			s.setPosition(it + sf::Vector2f(32.0f, 32.0f));
			r.add(s, RenderManager::INDEX_SAILS);
		}
	}

}
void ShipRenderer::reset()
{
	if (auto cont = std::dynamic_pointer_cast<ShipController>(this->getParent().lock()->components.controller)) {
		this->swivelAngle = cont->getSwivelAngle();
	}
	this->setSprites();
}

void ShipRenderer::setSprites()
{
	this->bigSail = ResourceManager::get()->getSprite(
		"ships",
		"sailLarge" + this->getSailColorString(this->sailColor) + this->getDamageString() + ".png",
		true);
	if (this->hasSmallSail) {
		// Small sails only have 1 destroyed sprite, rather than 1 for each color
		// So if the small sail is destroyed, don't have a color string
		std::string colorName = this->getSailColorString(this->sailColor);
		if (this->getParent().lock()->components.health != nullptr && this->getParent().lock()->components.health->getHealth() == 0)
			colorName = "";
		this->smallSail = ResourceManager::get()->getSprite(
			"ships",
			"sailSmall" + colorName + this->getDamageString() + ".png",
			true
		);
	}
	this->hull = ResourceManager::get()->getSprite(
		"ships",
		"hullLarge" + this->getDamageString() + ".png",
		true
	);
}
std::string ShipRenderer::getDamageString()
{
	int health;
	if (this->getParent().lock()->components.health == nullptr)
		health = 100;
	else
		health = this->getParent().lock()->components.health->getHealth();

	if (health == 100)
		return "";
	else if (health > 50)
		return "Damaged";
	else if (health > 0)
		return "Damaged2";
	else
		return "Destroyed";

}