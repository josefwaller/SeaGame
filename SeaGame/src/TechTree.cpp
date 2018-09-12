#include "TechTree.h"

std::map<Technology, TechTreeNode> TechTree::nodes = {
	{ 
		Technology::MakingStuff,
		{
			Technology::Nothing,
			"MAking stuff",
			false,
		},
	},
	{
		Technology::MakingStuff2,
		{
			Technology::MakingStuff,
			"Making Stuff 2",
			false
		}
	}
};