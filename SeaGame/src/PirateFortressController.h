#pragma once
#include "BaseController.h"

class PirateFortressController : public BaseController {
public:
	static const float SPAWN_DELAY;
	PirateFortressController();
	virtual void update(float delta) override;
private:
	sf::Clock spawnTimer;
	std::weak_ptr<Entity> ship;
};
