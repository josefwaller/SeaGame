#pragma once
#include <memory>
#include <map>

// Forward declaration of Entity
struct Entity;

class Component
{
public:
	Component(std::weak_ptr<Entity> parent);
	virtual std::weak_ptr<Entity> getParent();
	virtual std::map<std::string, std::string> getSaveData();
	// Runs after all entities have been added, thus can reference
	// entities by id
	virtual void fromSaveData(std::map<std::string, std::string> data);
private:
	std::weak_ptr<Entity> parent;
};

#include "Entity.h"