#include "ResourceController.h"

ResourceController::ResourceController(std::weak_ptr<Entity> parent, GameResource res, unsigned int count)
	: ControllerComponent(parent) {
	this->res = res;
	this->count = count;
}

GameResource ResourceController::getResource() {
	return this->res;
}
unsigned int ResourceController::getCount() {
	return this->count;
}

void ResourceController::update(float delta) {

}
