#include "GameResource.h"

std::string getResourceString(GameResource res) {
	switch (res) {
	case GameResource::Gold: return "Gold";
	case GameResource::Wood: return "Wood";
	case GameResource::Stone: return "Stone";
	case GameResource::Plank: return "Plank";
	case GameResource::StoneBrick: return "Stone Bricks";
	case GameResource::StoneStatue: return "Statue";
	case GameResource::Copper: return "Copper Ore";
	case GameResource::Iron: return "Iron Ore";
	}
	return "Missing Resource String (oh nooooooo)";
}
