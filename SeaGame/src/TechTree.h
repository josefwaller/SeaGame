#pragma once
#include <map>
#include "GameResource.h"

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
	Metallurgy,
	Forestries,
	Woodcutting,
	ShipBuilding,
	Furniture,
	Farming,
	Mills,
	Orchards,
	Breweries,
	MilitaryBases,
	UpgradedCannons,
	MilitaryShips,
	FollowingMilitryShips
};

struct TechTreeNode {
	Technology parent;
	std::string name;
	// The amount of money needed to discover this resource
	unsigned int cost;
	bool isResearched;
	TechTreeNode() {}
	TechTreeNode(const char* name, Technology parent, unsigned int cost) {
		this->name = name;
		this->parent = parent;
		this->cost = cost;
		this->isResearched = false;
	}
};
class TechTree {
public:
	static std::map<Technology, TechTreeNode> nodes;
};