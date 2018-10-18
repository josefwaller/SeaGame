#pragma once
#include <map>
#include "GameResource.h"

// The different tachnologies in the game
enum Technology {
	// Used to signal that a technology has no prerequisite
	// Cannot actually be researched
	Nothing,
	MakingStuff,
	MakingStuff2
};

struct TechTreeNode {
	Technology parent;
	std::string name;
	// The amount of money needed to discover this resource
	unsigned int cost;
	bool isResearched;
};
class TechTree {
public:
	static std::map<Technology, TechTreeNode> nodes;
};