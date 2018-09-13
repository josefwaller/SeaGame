#pragma once
#include <map>
#include "GameResource.h"

enum Technology {
	// Used to signal that a technology has no prerequisite
	Nothing,
	MakingStuff,
	MakingStuff2
};

struct TechTreeNode {
	Technology parent;
	std::string name;
	// The resources needed to research this technology
	std::map<GameResource, unsigned int> requiredResources;
	bool isResearched;
};
class TechTree {
public:
	static std::map<Technology, TechTreeNode> nodes;
};