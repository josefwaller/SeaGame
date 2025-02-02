#pragma once
#include "BaseController.h"
#include "GameResource.h"

/*
 * Base which generates a resource over time
 * Todo: rename
 */
class GenerationBaseController : public BaseController {
public:
	GenerationBaseController(GameResource res);
	virtual void update(float deltaTime) override;
	// Get the resource this base generates
	GameResource getResource();
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;
	virtual tgui::Widget::Ptr getGui() override;
protected:
	virtual void addResource();
private:
	static const float GENERATION_DELAY;
	GameResource res;
	sf::Clock resourceClock;
	// The gui of how far the generation is
	tgui::ProgressBar::Ptr progressGui;
};