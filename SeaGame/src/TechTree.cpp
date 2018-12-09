#include "TechTree.h"
#define TIER_1_COST 500
#define TIER_2_COST 1000
#define TIER_3_COST 2000
#define TIER_4_COST 2500
#define TIER_5_COST 10000


std::map<Technology, TechTreeNode> TechTree::nodes = {
	{
		Technology::Ferries,
		TechTreeNode("Ferries", Technology::Nothing, TIER_1_COST)
	},
	{
		Technology::MultipleStops,
		TechTreeNode("Multiple Stops for Ferries", Technology::Ferries, TIER_2_COST)
	},
	{
		Technology::FasterFerries,
		TechTreeNode("Fast Ferries", Technology::MultipleStops, TIER_3_COST)
	},
	{
		Technology::Mines,
		TechTreeNode("Mines", Technology::Nothing, TIER_1_COST)
	},
	{
		Technology::IronMining,
		TechTreeNode("Iron Mines", Technology::Mines, TIER_2_COST)
	},
	{
		Technology::CopperMining,
		TechTreeNode("Copper Mines", Technology::Mines, TIER_2_COST)
	},
	{
		Technology::GoldMining,
		TechTreeNode("Gold Mining", Technology::IronMining, TIER_2_COST)
	},
	{
		Technology::Counterfeiting,
		TechTreeNode("Counterfeiting", Technology::GoldMining, TIER_3_COST)
	}
};