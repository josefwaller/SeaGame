#pragma once
#include <memory>
#include <map>
#include <TGUI/Widgets/Panel.hpp>

// Forward declaration of Entity
struct Entity;
class Game;
class SaveData;

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
	virtual tgui::Widget::Ptr getGui();
protected:
	// Get game the entity exists in
	Game* getGame();
private:
	std::weak_ptr<Entity> parent;
};

#include "Entity.h"
#include "Game.h"