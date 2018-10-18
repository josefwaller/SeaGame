#include "TechTree.h"

std::map<Technology, TechTreeNode> TechTree::nodes = {
	{ 
		Technology::MakingStuff,
		{
			Technology::Nothing,
			"MAking stuff",
			250,
			false,
		},
	},
	{
		Technology::MakingStuff2,
		{
			Technology::MakingStuff,
			"Making Stuff 2",
			500,
			true
		}
	}
};