#include "Entity.h"
#include "Game.h"
#include "ConversionBaseController.h"
#include <algorithm>

unsigned int Entity::trueEntityCount = 0;

unsigned long Entity::currentId = 0;
std::map<std::string, std::string> Entity::getSaveData() {
	std::map<std::string, std::string> data = {
		{"type", std::to_string((int)this->type)},
		{"id", std::to_string(this->id)}
	};
	// Iterate over a pointer of each component this->entity has
	for (ComponentType t : ComponentList::allTypes) {
		if (auto c = this->components.get(t)) {
			auto m = c->getSaveData();
			data.insert(m.begin(), m.end());
		}
	}
	return data;
}
std::string Entity::getStringRep() {
	std::shared_ptr<ConversionBaseController> cont;
	switch (this->type) {
	case EntityType::Player: return "Player";
	case EntityType::CannonBall: return "Cannon Ball";
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