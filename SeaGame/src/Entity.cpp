#include "Entity.h"
#include "Game.h"
#include "GenerationBaseController.h"
#include "ConversionBaseController.h"
#include <algorithm>
#include "ComponentList.h"
#include "SaveData.h"

unsigned int Entity::trueEntityCount = 0;
unsigned long Entity::currentId = 0;
// Empty constuctor
Entity::Entity() : game(nullptr) {}
// Construct with game, team, type, tag and components
Entity::Entity(Game* g, int team, EntityType type, EntityTag tag, ComponentList c)
	: game(g), team(team), type(type), tag(tag), components(c) {
	// Set id
	Entity::trueEntityCount++;
	this->id = Entity::currentId;
	Entity::currentId++;
}
Entity::~Entity() {
	Entity::trueEntityCount--;
}
SaveData Entity::getSaveData() {
	// Initialize save data
	SaveData sd("Entity", {
		{"type", std::to_string((int)this->type)},
		{"tag", std::to_string((int)this->tag)},
		{"id", std::to_string(this->id)},
		{"team", std::to_string(this->team)}
	});
	// Get data for all components
	for (ComponentType t : ComponentList::allTypes) {
		if (this->components.get(t)) {
			SaveData cData = this->components.get(t)->getSaveData();
			cData.addValue("type", ComponentList::ComponentStrings[t]);
			sd.addData(cData);
		}
	}
	return sd;
}
std::string Entity::getStringRep() {
	std::shared_ptr<ConversionBaseController> cont;
	std::shared_ptr<GenerationBaseController> mCont;
	switch (this->type) {
	case EntityType::Player: return "Player";
	case EntityType::Cannonball: return "Cannon Ball";
	case EntityType::PirateShip: return "Pirate Ship";
	case EntityType::MilitaryBase: return "Defense Base";
	case EntityType::ForestryBase: return "Forestry";
	case EntityType::Ferry: return "Ferry";
	case EntityType::City: return "City";
	case EntityType::PirateBase: return "Pirate Outpost";
	case EntityType::ResourceDeposit: return "Resource Deposit";
	case EntityType::MiningBase:
		mCont = std::dynamic_pointer_cast<GenerationBaseController>(this->components.controller);
		switch (mCont->getResource()) {
		case GameResource::Stone: return "Quary";
		case GameResource::Wood: return "Forestry";
		case GameResource::Wheat: return "Farm";
		case GameResource::Fruit: return "Orchard";
		default: return "Missing string for generation base";
		}
		break;
	case EntityType::ConversionBase:
		cont = std::dynamic_pointer_cast<ConversionBaseController>(this->components.controller);
		return cont->getRecipe().baseName;
	default: return "Missing string for entity";
	}
}