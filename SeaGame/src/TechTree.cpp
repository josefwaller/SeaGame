#include "TechTree.h"
#define TIER_1_COST 500
#define TIER_2_COST 1000
#define TIER_3_COST 2000
#define TIER_4_COST 2500
#define TIER_5_COST 10000


std::map<Technology, TechTreeNode> TechTree::nodes = {
	{
		Technology::Ferries,
		TechTreeNode("Ferries", Technology::Nothing, TIER_1_COST,
		"Build ferries to bring resources between two bases")
	},
	{
		Technology::MultipleStops,
		TechTreeNode("Multiple Stops for Ferries", Technology::Ferries, TIER_2_COST,
		"Allow ferries to stop at more than two bases per route")
	},
	{
		Technology::FasterFerries,
		TechTreeNode("Fast Ferries", Technology::MultipleStops, TIER_3_COST,
		"Increases the speed of ferries")
	},
	{
		Technology::Mines,
		TechTreeNode("Mines", Technology::Nothing, TIER_1_COST,
		"Build stone mines to gather stone automatically")
	},
	{
		Technology::IronMining,
		TechTreeNode("Iron Mines", Technology::Mines, TIER_2_COST,
		"Build iron mines to gather iron automatically")
	},
	{
		Technology::CopperMining,
		TechTreeNode("Copper Mines", Technology::Mines, TIER_2_COST,
		"Build copper mines to gather copper automatically")
	},
	{
		Technology::GoldMining,
		TechTreeNode("Gold Mining", Technology::IronMining, TIER_2_COST,
		"Build gold mines to gather gold automatically")
	},
	{
		Technology::Counterfeiting,
		TechTreeNode("Counterfeiting", Technology::GoldMining, TIER_3_COST,
		"Build bases that turn gold bars into money at a high exchange rate")
	},
	{
		Technology::Forestries,
		TechTreeNode("Forestries", Technology::Nothing, TIER_1_COST,
		"Build bases that automatically gather wood from trees")
	},
	{
		Technology::Woodcutting,
		TechTreeNode("Woodcutting", Technology::Forestries, TIER_2_COST,
		"Build bases that turn wood into planks")
	},
	{
		Technology::Farming,
		TechTreeNode("Farming", Technology::Forestries, TIER_2_COST,
		"Build farms, which don't require any resource to be build and generates wheat")
	},
	{
		Technology::Mills,
		TechTreeNode("Milling", Technology::Farming, TIER_3_COST,
		"Build bases to grind wheat into flour")
	},
	{
		Technology::Baking,
		TechTreeNode("Baking", Technology::Mills, TIER_4_COST,
		"Build bases that bake wheat into bread that can be sold at a high price")
	}
};