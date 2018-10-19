#pragma once
#include "BaseController.h"
#include "ConversionRecipes.h"

class ConversionBaseController : public BaseController {
public:
	ConversionBaseController(std::weak_ptr<Entity> parent, GameResource product);
	virtual void update(float delta) override;
private:
	sf::Clock conversionClock;
	// What the base is going to make
	GameResource product;
};