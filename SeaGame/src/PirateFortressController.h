#pragma once
#include "BaseController.h"

class PirateFortressController : public BaseController {
public:
	PirateFortressController();
	virtual void update(float delta) override;
};
