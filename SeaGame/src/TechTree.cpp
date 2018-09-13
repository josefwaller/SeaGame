#include "TechTree.h"

std::map<Technology, TechTreeNode> TechTree::nodes = {
	{ 
		Technology::MakingStuff,
		{
			Technology::Nothing,
			"MAking stuff",
			{
				{ GameResource::Gold, 150 }
			},
			false,
		},
	},
	{
		Technology::MakingStuff2,
		{
			Technology::MakingStuff,
			"Making Stuff 2",
			{
				{ GameResource::Gold, 200 }
			},
			false
		}
	}
};