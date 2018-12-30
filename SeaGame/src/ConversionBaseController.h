#pragma once
#include "BaseController.h"
#include "ConversionRecipes.h"

class ConversionBaseController : public BaseController {
public:
	ConversionBaseController( GameResource product);
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void update(float delta) override;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;
	GameResource getProduct();
private:
	float sinceLastConversion;
	// What the base is going to make
	GameResource product;
};