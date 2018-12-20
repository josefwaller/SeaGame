#pragma once
#include "BaseController.h"

/*
 * Base which generates a resource over time
 * Todo: rename
 */
class MiningBaseController : public BaseController {
public:
	MiningBaseController(GameResource res);
	virtual void update(float deltaTime) override;
	// Get the resource this base generates
	GameResource getResource();
private:
	static const float GENERATION_DELAY;
	GameResource res;
	sf::Clock resourceClock;
};