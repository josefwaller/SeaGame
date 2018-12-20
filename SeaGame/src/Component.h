#pragma once
#include <memory>
#include <map>
#include <TGUI/TGUI.hpp>

// Forward declaration of Entity
struct Entity;

class Component
{
public:
	Component();
	// Set the parent
	// May be overridden by a component to allow set up after all other components are initialized
	virtual void setParent(std::weak_ptr<Entity> parent);
	// Get the parent
	virtual std::weak_ptr<Entity> getParent();
	virtual std::map<std::string, std::string> getSaveData();
	// Runs after all entities have been added, thus can reference
	// entities by id
	virtual void fromSaveData(std::map<std::string, std::string> data);
	// Add a panel to the entitie's GUI
	virtual void updateGui(tgui::Tabs::Ptr tabs, std::map<std::string, tgui::Panel::Ptr>* panels);
private:
	std::weak_ptr<Entity> parent;
};

#include "Entity.h"