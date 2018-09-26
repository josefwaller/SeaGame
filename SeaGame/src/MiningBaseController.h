#pragma once
#include "BaseController.h"

class MiningBaseController : public BaseController {
public:
	MiningBaseController(std::weak_ptr<Entity> parent, GameResource res);
	virtual void update(float deltaTime) override;
private:
	static const float GENERATION_DELAY;
	GameResource res;
	sf::Clock resourceClock;
};