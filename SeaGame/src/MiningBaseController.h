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
	virtual std::map<std::string, std::string> getSaveData() override;
	virtual void fromSaveData(std::map<std::string, std::string> data) override;
private:
	static const float GENERATION_DELAY;
	GameResource res;
	sf::Clock resourceClock;
};