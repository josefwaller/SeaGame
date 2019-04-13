#pragma once
#include <map>
#include "GameResource.h"
class SaveData;

// The different tachnologies in the game
enum Technology {
	// Used to signal that a technology has no prerequisite
	// Cannot actually be researched
	Nothing,
	Ferries,
	MultipleStops,
	FasterFerries,
	Mines,
	IronMining,
	CopperMining,
	GoldMining,
	Counterfeiting,
	Smelting,
	Forgery,
	Forestries,
	Woodcutting,
	ShipBuilding,
	Carpentry,
	Farming,
	Mills,
	Baking,
	Orchards,
	Breweries,
	MilitaryBases,
	UpgradedCannons,
	MilitaryShips,
	Fleets,
	EscapeShip,
	DeepMining,
	SustainableForests
};

struct TechTreeNode {
	Technology parent;
	std::string name;
	std::string description;
	// The amount of money needed to discover this resource
	unsigned int cost;
	bool isResearched;
	TechTreeNode() {}
	TechTreeNode(const char* name, Technology parent, unsigned int cost, std::string description) {
		this->name = name;
		this->parent = parent;
		this->cost = cost;
		this->isResearched = false;
		this->description = description;
	}
};
struct TechTree {
	TechTree();
	std::map<Technology, TechTreeNode> nodes;
};