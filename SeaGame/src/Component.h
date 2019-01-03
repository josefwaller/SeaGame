#pragma once
#include <memory>
#include <map>
#include <TGUI/TGUI.hpp>
#include "SaveData.h"

// Forward declaration of Entity
struct Entity;

class Component
{
public:
	Component();
	virtual ~Component();
	// Set the parent
	// May be overridden by a component to allow set up after all other components are initialized
	virtual void setParent(std::weak_ptr<Entity> parent);
	// Get the parent
	virtual std::weak_ptr<Entity> getParent();
	virtual SaveData getSaveData();
	// Runs after all entities have been added, thus can reference entities by id
	virtual void fromSaveData(SaveData data);
	// Add a panel to the entitie's GUI
	virtual void updateGui(tgui::Tabs::Ptr tabs, std::map<std::string, tgui::Panel::Ptr>* panels);
private:
	std::weak_ptr<Entity> parent;
};

#include "Entity.h"