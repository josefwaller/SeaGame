#include "ResourceController.h"

ResourceController::ResourceController(GameResource res, unsigned int count)
	: ControllerComponent() {
	this->res = res;
	this->count = count;
}
void ResourceController::onClick() {
	this->getParent().lock()->game->getPlayer()->components.inventory->addItems(this->res, 1);
	this->count--;
}

GameResource ResourceController::getResource() {
	return this->res;
}
unsigned int ResourceController::getCount() {
	return this->count;
}

void ResourceController::update(float delta) {

}

SaveData ResourceController::getSaveData() {
	return SaveData("Component", {
		{ "res", std::to_string((int)this->res) },
		{ "count", std::to_string(this->count) }
	});
}
void ResourceController::fromSaveData(SaveData data) {
	this->res = (GameResource)std::stoi(data.getValue("res"));
	this->count = std::stoi(data.getValue("count"));
}