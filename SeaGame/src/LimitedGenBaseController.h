#pragma once
#include <TGUI/Widget.hpp>
#include <TGUI/Widgets/Label.hpp>
#include "GenerationBaseController.h"

/*
 * A Generation base with a limit on how many resources it can generate
 */
class LimitedGenBaseController : public GenerationBaseController {
public:
	LimitedGenBaseController(GameResource res, unsigned int limit);
	virtual void update(float delta) override;
	virtual void addResource() override;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;
	virtual tgui::Widget::Ptr getGui() override;
private:
	unsigned int amountLeft;
	tgui::Label::Ptr amountLeftLabel;
};
