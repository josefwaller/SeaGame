#include "ExportShipController.h"
#include "BaseController.h"
#include "Entity.h"

const sf::Vector2f ExportShipController::OFFSCREEN_TARGET = { 0.0f, 0.0f };

ExportShipController::ExportShipController(std::weak_ptr<Entity> b) {
	this->base = b;
	this->goingToBase = false;
}
void ExportShipController::setParent(std::weak_ptr<Entity> p) {
	AutomatedShipController::setParent(p);
	this->setTarget(OFFSCREEN_TARGET);
}
void ExportShipController::update(float delta) {
	this->move(delta);
}
void ExportShipController::onReachingTarget() {
	if (!this->goingToBase) {
		this->setTarget(
			std::dynamic_pointer_cast<BaseController>(
				this->base.lock()->components.controller
				)->getDockCoords()
		);
	}
	else {
		this->setTarget(OFFSCREEN_TARGET);
	}
	this->goingToBase = !this->goingToBase;
}