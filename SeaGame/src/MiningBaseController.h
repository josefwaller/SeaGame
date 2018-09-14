#pragma once
#include "ControllerComponent.h"

class MiningBaseController : public ControllerComponent {
public:
	MiningBaseController(std::weak_ptr<Entity> parent, GameResource res);
	virtual void update(float deltaTime) override;
private:
	static const float GENERATION_DELAY;
	GameResource res;
	sf::Clock resourceClock;
};