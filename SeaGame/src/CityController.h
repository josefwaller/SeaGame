#pragma once
#include "BaseController.h"

class CityController : public BaseController {
public:
	CityController(std::weak_ptr<Entity> parent);
	virtual void update(float delta) override;
};