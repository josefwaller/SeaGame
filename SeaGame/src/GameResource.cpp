#include "GameResource.h"
#include "ResourceManager.h"

std::string getResourceString(GameResource res) {
	return RESOURCE_STRINGS.find(res)->second;
}
sf::Sprite getResourceSprite(GameResource res, bool center) {
	std::string spriteName;
	switch (res) {
	case GameResource::Stone:
		spriteName = "medievalEnvironment_08.png";
		break;
	case GameResource::Gold:
		spriteName = "medievalEnvironment_11.png";
		break;
	case GameResource::Wood:
		spriteName = "medievalEnvironment_03.png";
		break;
	case GameResource::Plank:
		spriteName = "medievalEnvironment_05.png";
		break;
	case GameResource::StoneBrick:
		spriteName = "medievalEnvironment_07.png";
		break;
	case GameResource::StoneStatue:
		spriteName = "medievalStructure_12.png";
		break;
	case GameResource::Copper:
		spriteName = "medievalEnvironment_10.png";
		break;
	case GameResource::Iron:
		spriteName = "medievalEnvironment_17.png";
		break;
	case GameResource::Wheat:
		spriteName = "medievalEnvironment_20.png";
		break;
	case GameResource::Flour:
		spriteName = "medievalStructure_21.png";
		break;
	case GameResource::Bread:
		spriteName = "medievalStructure_03.png";
		break;
	}
	return ResourceManager::get()->getSprite("medievalRTS_spritesheet@2", spriteName, center);
}

bool generationBaseNeedsSource(GameResource res) {
	std::vector<GameResource> need = {
		GameResource::Stone,
		GameResource::Iron,
		GameResource::Gold,
		GameResource::Copper,
		GameResource::Wood
	};
	return std::find(need.begin(), need.end(), res) != need.end();
}