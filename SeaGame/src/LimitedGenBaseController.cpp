#include "LimitedGenBaseController.h"
#include "GameResource.h"
#include "SaveData.h"
#include <TGUI/Container.hpp>
#include <memory>

LimitedGenBaseController::LimitedGenBaseController(GameResource res, unsigned int limit)
	: GenerationBaseController(res) {
	this->amountLeft = limit;
	this->amountLeftLabel = tgui::Label::create();
}
void LimitedGenBaseController::update(float delta) {
	if (this->amountLeft > 0) {
		GenerationBaseController::update(delta);
	}
}
void LimitedGenBaseController::addResource() {
	GenerationBaseController::addResource();
	// Decrease the amount left
	this->amountLeft--;
	std::string amountLeftText = "Amount Left: " + std::to_string(this->amountLeft);
	this->amountLeftLabel->setText(amountLeftText);
}
SaveData LimitedGenBaseController::getSaveData() {
	SaveData data = GenerationBaseController::getSaveData();
	data.addValue("amountLeft", std::to_string(this->amountLeft));
	return data;
}
void LimitedGenBaseController::fromSaveData(SaveData data) {
	GenerationBaseController::fromSaveData(data);
	this->amountLeft = std::stoi(data.getValue("amontLeft"));
}
tgui::Widget::Ptr LimitedGenBaseController::getGui() {
	auto p = GenerationBaseController::getGui();
	this->amountLeftLabel->setPosition(0, 30.0f);
	std::dynamic_pointer_cast<tgui::Container>(p)->add(this->amountLeftLabel);
	return p;
}