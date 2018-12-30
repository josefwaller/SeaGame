#include "MiningBaseController.h"
#include "GameResource.h"

const float MiningBaseController::GENERATION_DELAY = 2000.0f;
MiningBaseController::MiningBaseController(GameResource res) : BaseController() {
	this->res = res;
}
void MiningBaseController::update(float delta) {
	if (this->resourceClock.getElapsedTime().asMilliseconds() > MiningBaseController::GENERATION_DELAY) {
		this->getParent().lock()->components.inventory->addItems(this->res, 1);
		this->resourceClock.restart();
	}
}
GameResource MiningBaseController::getResource() {
	return this->res;
}
SaveData MiningBaseController::getSaveData() {
	return SaveData("Component", {
		{"product", std::to_string(this->res)}
	});
}
void MiningBaseController::fromSaveData(SaveData data) {
	this->res = (GameResource)(std::stoi(data.getValue("product")));
}