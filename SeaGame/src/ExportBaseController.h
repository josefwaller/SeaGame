#pragma once
#include "BaseController.h"

class ExportBaseController : public BaseController {
public:
	virtual void update(float delta) override;
private:
	// The ship that carries the base's goods away to be sold
	std::weak_ptr<Entity> ship;
};
