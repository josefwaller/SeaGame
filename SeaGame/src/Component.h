#pragma once
#include <memory>
#include <map>
#include <TGUI/Widgets/Panel.hpp>

// Forward declaration of Entity
struct Entity;
class Game;
class SaveData;
struct ComponentList;

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
	// Get the component list of this entity
	// Cuts down on the number of entities that need to include "Entity.h"
	ComponentList getComponentList();
private:
	std::weak_ptr<Entity> parent;
};

// Since nearly every entity uses getComponetList and getGame, we just include
// game and component list here.
#include "ComponentList.h"
#include "Game.h"