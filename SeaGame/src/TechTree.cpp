#include "TechTree.h"
#include "SaveData.h"
#define TIER_1_COST 500
#define TIER_2_COST 1000
#define TIER_3_COST 2000
#define TIER_4_COST 2500
#define TIER_5_COST 1000000


// Creating a new tech tree, set up all technologies to default
TechTree::TechTree() {
	this->nodes = {
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
			Technology::Carpentry,
			TechTreeNode("Carpentry", Technology::Woodcutting, TIER_3_COST,
			"Buil bases to turn planks into more valuable furniture")
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
		},
		{
			Technology::Orchards,
			TechTreeNode("Fruit Cultivation", Technology::Farming, TIER_3_COST,
			"Build orchards that grow fruit to be sold or brewed into beer")
		},
		{
			Technology::Breweries,
			TechTreeNode("Breweries", Technology::Orchards, TIER_4_COST,
			"Brew fruits into beer, which sells for a high price")
		},
		{
			Technology::Smelting,
			TechTreeNode("Smelting", Technology::IronMining, TIER_2_COST,
			"Smelt Iron into steel")
		},
		{
			Technology::Forgery,
			TechTreeNode("Forging", Technology::Smelting, TIER_3_COST,
			"Build forges to turn Steel into weapons")
		},
		{
			Technology::MilitaryBases,
			TechTreeNode("Military Bases", Technology::Nothing, TIER_1_COST,
			"Allows the creationg of military bases, which defend against pirates")
		},
		{
			Technology::MilitaryShips,
			TechTreeNode("Military Ships", Technology::MilitaryBases, TIER_2_COST,
			"Creates Military Ships, which can be assigned to a Ferry or to the player")
		},
		{
			Technology::Fleets,
			TechTreeNode("Fleets", Technology::MilitaryShips, TIER_3_COST,
			"Allows creating a leader ship with a target, which other military ships can be assigned to.")
		},
		{
			Technology::DeepMining,
			TechTreeNode("Deep Mining", Technology::Mines, TIER_4_COST,
			"Allows creating a deep mining base which never runs out of resources")
		},
		{
			Technology::SustainableForests,
			TechTreeNode("Sustainable Forestry", Technology::Forestries, TIER_4_COST,
			"Allows creating a forestry which never runs out of wood")
		},
		{
			Technology::Exports,
			TechTreeNode("Exports", Technology::FasterFerries, TIER_5_COST,
			"Allows building bases which sends resources back home for a lot of money. Building this is your end goal.")
		}
	};
}