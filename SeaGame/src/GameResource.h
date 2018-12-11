#pragma once
#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

enum GameResource
{
	Stone,
	Wood,
	Gold,
	Plank,
	Iron,
	Copper,
	StoneBrick,
	StoneStatue
};

const std::vector<GameResource> ALL_RESOURCES = {
	GameResource::Stone,
	GameResource::Wood,
	GameResource::Gold,
	GameResource::Plank,
	GameResource::StoneBrick,
	GameResource::StoneStatue
};

// How much the player gets for selling these resources
const std::map<GameResource, unsigned int> RESOURCE_VALUES = {
	{GameResource::Stone, 10},
	{GameResource::Wood, 10},
	{GameResource::Gold, 100},
	{GameResource::Plank, 35},
	{GameResource::StoneBrick, 30},
	{GameResource::StoneStatue, 200}
};

std::string getResourceString(GameResource res);
sf::Sprite getResourceSprite(GameResource res, bool center = true);