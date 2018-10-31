#pragma once
#include <string>

enum GameResource
{
	Stone,
	Wood,
	Gold,
	Plank,
	StoneBrick,
	StoneStatue
};

std::string getResourceString(GameResource res);