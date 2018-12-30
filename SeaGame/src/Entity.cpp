#include "Entity.h"
#include "Game.h"
#include "ConversionBaseController.h"
#include <algorithm>

unsigned int Entity::trueEntityCount = 0;

unsigned long Entity::currentId = 0;
SaveData Entity::getSaveData() {
	// Initialize save data
	SaveData sd("Entity", {
		{"type", std::to_string((int)this->type)},
		{"tag", std::to_string((int)this->tag)},
		{"id", std::to_string(this->id)}
	});
	// Get data for all components
	for (ComponentType t : ComponentList::allTypes) {
		if (this->components.get(t)) {
			SaveData cData = this->components.get(t)->getSaveData();
			cData.addValue("type", std::to_string(t));
			sd.addData(cData);
		}
	}
	return sd;
}
std::string Entity::getStringRep() {
	std::shared_ptr<ConversionBaseController> cont;
	switch (this->type) {
	case EntityType::Player: return "Player";
	case EntityType::Cannonball: return "Cannon Ball";
	case EntityType::PirateShip: return "Pirate Ship";
	case EntityType::MilitaryBase: return "Defense Base";
	case EntityType::MiningBase: return "Quary";
	case EntityType::ForestryBase: return "Forestry";
	case EntityType::Ferry: return "Ferry";
	case EntityType::City: return "City";
	case EntityType::PirateBase: return "Pirate Outpost";
	case EntityType::ConversionBase:
		cont = std::dynamic_pointer_cast<ConversionBaseController>(this->components.controller);
		switch (cont->getProduct()) {
		case GameResource::Plank: return "Wood Cutters";
		case GameResource::StoneBrick: return "Stone Cutters";
		case GameResource::StoneStatue: return "Statue Makers";
		default: return "";
		}
	default: return "";
	}
}