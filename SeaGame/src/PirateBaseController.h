#pragma once
#include "ControllerComponent.h"

class PirateBaseController : public ControllerComponent {
public:
	PirateBaseController(std::weak_ptr<Entity> parent);
	virtual void update(float delta) override;
	static float SPAWN_DELAY;
private:
	sf::Clock spawnTimer;
	void spawnShip();
};