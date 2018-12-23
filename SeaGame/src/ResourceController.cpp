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
