#include "MiningBaseController.h"
#include "InventoryComponent.h"
#include "GameResource.h"
#include "SaveData.h"

const float MiningBaseController::GENERATION_DELAY = 2000.0f;
MiningBaseController::MiningBaseController(GameResource res) : BaseController() {
	this->res = res;
	this->progressGui = tgui::ProgressBar::create();
}
void MiningBaseController::update(float delta) {
	if (this->resourceClock.getElapsedTime().asMilliseconds() > MiningBaseController::GENERATION_DELAY) {
		this->getParent().lock()->components.inventory->addItems(this->res, 1);
		this->resourceClock.restart();
	}
	this->progressGui->setValue(100 * this->resourceClock.getElapsedTime().asMilliseconds() / GENERATION_DELAY);
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
tgui::Widget::Ptr MiningBaseController::getGui() {
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