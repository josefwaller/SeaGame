#pragma once
#include <map>

enum Technology {
	// Used to signal that a technology has no prerequisite
	Nothing,
	MakingStuff,
	MakingStuff2
};

struct TechTreeNode {
	Technology parent;
	std::string name;
	bool isResearched;
};
class TechTree {
public:
	static std::map<Technology, TechTreeNode> nodes;
};