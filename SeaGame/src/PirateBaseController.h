#pragma once
#include "BaseController.h"

class PirateBaseController : public BaseController {
public:
	PirateBaseController();
	virtual void update(float delta) override;
	static float SPAWN_DELAY;
	// How many ships the base can have out at once
	static unsigned int MAX_SHIPS;
private:
	sf::Clock spawnTimer;
	// The current number of ships the base has out
	unsigned int numShips;
	void spawnShip();
};