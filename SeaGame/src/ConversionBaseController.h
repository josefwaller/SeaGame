#pragma once
#include "BaseController.h"
#include "ConversionRecipes.h"

class ConversionBaseController : public BaseController {
public:
	ConversionBaseController(std::weak_ptr<Entity> parent, GameResource product);
	virtual void update(float delta) override;
	virtual std::map<std::string, std::string> getSaveData() override;
	virtual void fromSaveData(std::map<std::string, std::string> data) override;
	GameResource getProduct();
private:
	float sinceLastConversion;
	// What the base is going to make
	GameResource product;
};