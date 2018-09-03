#pragma once
#include "ControllerComponent.h"

class MiningBaseController : public ControllerComponent {
public:
	MiningBaseController(std::weak_ptr<Entity> parent);
	virtual void update(float deltaTime) override;
private:
	static const float GENERATION_DELAY;
	sf::Clock resourceClock;
};