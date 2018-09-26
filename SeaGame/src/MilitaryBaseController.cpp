#include "MilitaryBaseController.h"
#include "EntityPrefabs.h"

const float MilitaryBaseController::SHOOT_DELAY = 10.0f;
const float MilitaryBaseController::SHOOT_RANGE = 500.0f;

MilitaryBaseController::MilitaryBaseController(std::weak_ptr<Entity> parent) : BaseController(parent)
{
	this->cannons = {
		Cannon(this->getParent().lock(), sf::Vector2f(1.5 * 64.0f, 1.5 * 64.0f), 0.0f, SHOOT_DELAY)
	};
}

void MilitaryBaseController::update(float delta)
{
	for (auto it = this->cannons.begin(); it != this->cannons.end(); it++) {
		it->autoAim(it->findTarget(SHOOT_RANGE));
		it->fire();
	}
}

std::vector<Cannon> MilitaryBaseController::getCannons()
{
	return this->cannons;
}