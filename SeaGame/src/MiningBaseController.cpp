#include "MiningBaseController.h"
#include "GameResource.h"

const float MiningBaseController::GENERATION_DELAY = 2000.0f;
MiningBaseController::MiningBaseController(std::weak_ptr<Entity> parent, GameResource res) : BaseController(parent) {
	this->res = res;
}
void MiningBaseController::update(float delta) {
	if (this->resourceClock.getElapsedTime().asMilliseconds() > MiningBaseController::GENERATION_DELAY) {
		this->getParent().lock()->components.inventory->addItems(this->res, 1);
		this->resourceClock.restart();
	}
}