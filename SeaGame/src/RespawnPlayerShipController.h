#pragma once
#include "PlayerShipController.h"

class RespawnPlayerShipController : public PlayerShipController {
public:
	static const float RESPAWN_DELAY;
	virtual void update(float delta) override;
private:
	sf::Clock c;
};
