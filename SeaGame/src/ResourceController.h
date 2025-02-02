#pragma once
#include "ControllerComponent.h"

// Contains the methods for getting the resources and amount left in this resource deposit
class ResourceController : public ControllerComponent {
public:
	ResourceController(GameResource res, unsigned int count);
	virtual void onClick() override;
	// Get the resource
	GameResource getResource();
	// Get the number of resources left
	unsigned int getCount();
	// empty, maybe will be filled or the class will be moved to somewhere else
	virtual void update(float delta) override;
	// Get the resource to save
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData data) override;
private:
	GameResource res;
	unsigned int count;

};
