#include "Entity.h"
#include "Game.h"
#include "MiningBaseController.h"
#include "ConversionBaseController.h"
#include <algorithm>
#include "ComponentList.h"

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
			cData.addValue("type", ComponentList::ComponentStrings[t]);
			sd.addData(cData);
		}
	}
	return sd;
}
std::string Entity::getStringRep() {
	std::shared_ptr<ConversionBaseController> cont;
	std::shared_ptr<MiningBaseController> mCont;
	switch (this->type) {
	case EntityType::Player: return "Player";
	case EntityType::Cannonball: return "Cannon Ball";
	case EntityType::PirateShip: return "Pirate Ship";
	case EntityType::MilitaryBase: return "Defense Base";
	case EntityType::ForestryBase: return "Forestry";
	case EntityType::Ferry: return "Ferry";
	case EntityType::City: return "City";
	case EntityType::PirateBase: return "Pirate Outpost";
	case EntityType::MiningBase:
		mCont = std::dynamic_pointer_cast<MiningBaseController>(this->components.controller);
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
		switch (cont->getProduct()) {
		case GameResource::Plank: return "Wood Cutters";
		case GameResource::StoneBrick: return "Stone Cutters";
		case GameResource::StoneStatue: return "Statue Makers";
		case GameResource::Beer: return "Brewery";
		default: return "Missing string for conversion base";
		}
	default: return "Missing string for entity";
	}
}