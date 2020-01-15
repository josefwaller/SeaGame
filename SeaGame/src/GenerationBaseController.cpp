#include "GenerationBaseController.h"
#include "InventoryComponent.h"
#include "GameResource.h"
#include "SaveData.h"

const float GenerationBaseController::GENERATION_DELAY = 2000.0f;
GenerationBaseController::GenerationBaseController(GameResource res) : BaseController() {
	this->res = res;
	this->progressGui = tgui::ProgressBar::create();
}
void GenerationBaseController::update(float delta) {
	if (this->resourceClock.getElapsedTime().asMilliseconds() > GenerationBaseController::GENERATION_DELAY) {
		this->addResource();
		this->resourceClock.restart();
	}
	this->progressGui->setValue(100 * this->resourceClock.getElapsedTime().asMilliseconds() / GENERATION_DELAY);
}
void GenerationBaseController::addResource() {
	this->getComponentList().inventory->addItems(this->res, 1);
}
GameResource GenerationBaseController::getResource() {
	return this->res;
}
SaveData GenerationBaseController::getSaveData() {
	return SaveData("Component", {
		{"product", std::to_string(this->res)}
	});
}
void GenerationBaseController::fromSaveData(SaveData data) {
	this->res = (GameResource)(std::stoi(data.getValue("product")));
}
tgui::Widget::Ptr GenerationBaseController::getGui() {
	auto panel = tgui::Panel::create();
	// Add label
	auto label = tgui::Label::create();
	label->setText("Progress: ");
	panel->add(label);
	// Add progress bar
	panel->add(this->progressGui);
	this->progressGui->setPosition(tgui::bindWidth(label), 0);
	return panel;
}