#pragma once
#include "BaseController.h"

class PirateBaseController : public BaseController {
public:
	PirateBaseController(std::weak_ptr<Entity> parent);
	virtual void update(float delta) override;
	static float SPAWN_DELAY;
private:
	sf::Clock spawnTimer;
	void spawnShip();
};