#pragma once
#include "ControllerComponent.h"

class CityController : public ControllerComponent {
public:
	CityController(std::weak_ptr<Entity> parent);
	virtual void update(float delta) override;
};