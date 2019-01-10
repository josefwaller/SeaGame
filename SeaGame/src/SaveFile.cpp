#include "SaveFile.h"
#include "Game.h"
#include "Entity.h"
#include "EntityPrefabs.h"
#include <fstream>
#include <memory>
#include "PlayerShipController.h"
#include <rapidxml\rapidxml_print.hpp>
#include <rapidxml\rapidxml_utils.hpp>

SaveFile::SaveFile(Game* g) {
	rapidxml::xml_document<> doc;
	auto node = doc.allocate_node(rapidxml::node_element, "Game");
	doc.append_node(node);
	// Get the GameMap's save data
	this->addData(g->getGameMap()->getSaveData(), node, &doc);
	// Add entity save data
	auto n = doc.allocate_node(rapidxml::node_element, "EntityList");
	node->append_node(n);
	for (auto e : g->getEntities()) {
		this->addData(e->getSaveData(), n, &doc);
	}
	// Add tech tree save data
	SaveData techData("TechTree");
	for (auto n : TechTree::nodes) {
		techData.addData(SaveData("TechTreeNode", {
			{ "tech", std::to_string(n.first) },
			{ "isResearched", std::to_string(n.second.isResearched) }
		}));
	}
	this->addData(techData, node, &doc);
	// Save doc data
	std::ostringstream os;
	os << doc;
	this->data = os.str();
}
SaveFile::SaveFile(std::string fileName) {
	rapidxml::file<> f(fileName.c_str());
	this->data = std::string(f.data());
}
void SaveFile::save(std::string fileName) {
	std::ofstream f;
	f.open("test.xml");
	f << this->data;
	f.close();

}
void SaveFile::addData(SaveData sd, rapidxml::xml_node<>* parent, rapidxml::xml_document<>* doc) {
	// First, create a node for the data
	const char* name = doc->allocate_string(sd.getName().c_str());
	auto node = doc->allocate_node(rapidxml::node_element, name);
	parent->append_node(node);
	// Add all the string values
	for (auto v : sd.getValues()) {
		char* a = doc->allocate_string(v.first.c_str());
		char* b = doc->allocate_string(v.second.c_str());
		auto attr = doc->allocate_attribute(a, b);
		node->append_attribute(attr);
	}
	// Add the children
	for (auto it : sd.getDatas()) {
		this->addData(it, node, doc);
	}
}
SaveData SaveFile::getData(rapidxml::xml_node<>* node) {
	// Create save data
	SaveData sd(node->name());
	// Add attributes
	for (auto attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
		if (attr->name() != "name") {
			sd.addValue(attr->name(), attr->value());
		}
	}
	// Add nested nodes
	for (auto child = node->first_node(); child != nullptr; child = child->next_sibling()) {
		sd.addData(this->getData(child));
	}
	return sd;
}
std::unique_ptr<Game> SaveFile::load(App* a) {
	// Create the game
	std::unique_ptr<Game> game(new Game(a));
	// Get pointer reference for creating entities
	Game* g = game.get();
	// Parse data
	rapidxml::xml_document<> doc;
	doc.parse<0>((char*)this->data.c_str());
	auto gameNode = doc.first_node("Game");
	// Create GameMap
	SaveData gm = this->getData(gameNode->first_node("GameMap"));
	GameMap gMap(g, gm);
	// Load all the entities' data
	std::vector<SaveData> entityDatas;
	auto eN = gameNode->first_node("EntityList");
	for (auto n = eN->first_node("Entity"); n != nullptr; n = n->next_sibling()) {
		entityDatas.push_back(this->getData(n));
	}
	// Vector of entities
	std::vector<std::shared_ptr<Entity>> entities;
	// The player, which is also in entieies
	std::weak_ptr<Entity> player;
	// Create the entities from the data
	for (auto it : entityDatas) {
		// Create entity
		std::shared_ptr<Entity> e = EntityPrefabs::getEntityFromSaveData(g, it);
		// Override id
		e->id = std::stoi(it.getValue("id"));
		// Add to list
		entities.push_back(e);
		// Set to player if it is
		if (entities.back()->components.controller != nullptr && std::dynamic_pointer_cast<PlayerShipController>(entities.back()->components.controller)) {
			player = entities.back();
			player.lock()->components.inventory->addItems(GameResource::Gold, 300);
		}
	}
	// Get the tech tree
	SaveData techTreeData = this->getData(gameNode->first_node("TechTree"));
	for (SaveData n : techTreeData.getDatas()) {
		Technology tech = (Technology)(std::stoi(n.getValue("tech")));
		bool isResearched = (bool)(std::stoi(n.getValue("isResearched")));
		TechTree::nodes[tech].isResearched = isResearched;
	}
	// Add values to game
	g->loadFromData(
		gMap,
		entities,
		player,
		TechTree()
	);
	// Now that all values have been added to the game, can set component data from save data
	for (size_t i = 0; i < entityDatas.size(); i++) {
		std::shared_ptr<Entity> e = g->getEntityById(std::stoi(entityDatas[i].getValue("id"))).lock();
		std::vector<SaveData> cDatas = entityDatas[i].getDatas();
		for (ComponentType c : ComponentList::allTypes) {
			SaveData data("Component");
			for (auto cd : cDatas) {
				if (std::stoi(cd.getValue("type")) == (int)(c)) {
					data = cd;
					break;
				}
			}
			std::shared_ptr<Component> comp = e->components.get(c);
			if (comp) {
				comp->fromSaveData(data);
			}
		}
	}
	// After that, reset all the renderers because they entities might have changed unexpectedly
	// Ex: Bases might move, but bases should never move in an actual game
	for (auto it = entities.begin(); it != entities.end(); it++) {
		(*it)->components.renderer->reset();
	}

	// Return the game
	return std::move(game);
}