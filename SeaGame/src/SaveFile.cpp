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
	// Get the GameMap's save data
	g->getGameMap()->addSaveData(&doc);
	// Add entity save data
	auto n = doc.allocate_node(rapidxml::node_element, "EntityList");
	for (auto e : g->getEntities()) {
		auto data = e->getSaveData();
		auto eN = doc.allocate_node(rapidxml::node_element, "Entity");
		for (auto it : data) {
			char* a = doc.allocate_string(it.first.c_str());
			char* b = doc.allocate_string(it.second.c_str());
			eN->append_attribute(
				doc.allocate_attribute(
					a, b
				)
			);
		}
		// Iterate over a pointer of each component this->entity has
		for (ComponentType t : ComponentList::allTypes) {
			if (auto c = e->components.get(t)) {
				int i = (int)t;
				auto cNode = doc.allocate_node(rapidxml::node_element, "Component");
				auto m = c->getSaveData();
				m.insert({ "type", std::to_string(i) });
				for (auto it = m.begin(); it != m.end(); it++) {
					char* a = doc.allocate_string(it->first.c_str());
					char* b = doc.allocate_string(it->second.c_str());
					cNode->append_attribute(doc.allocate_attribute(a, b));
				}
				eN->append_node(cNode);
			}
		}
		n->append_node(eN);
	}
	doc.append_node(n);
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
std::unique_ptr<Game> SaveFile::load(App* a) {
	// Create the game
	std::unique_ptr<Game> game(new Game(a));
	// Get pointer reference for creating entities
	Game* g = game.get();
	// Parse data
	rapidxml::xml_document<> doc;
	doc.parse<0>((char*)this->data.c_str());
	// Create GameMap
	GameMap gMap(g, &doc);
	// Load all the entities' data
	std::vector<std::map<std::string, std::string>> entityDatas;
	std::vector<std::vector<std::map<std::string, std::string>>> componentDatas;
	auto eN = doc.first_node("EntityList");
	for (auto n = eN->first_node("Entity"); n != nullptr; n = n->next_sibling()) {
		// Load the attributes from the node
		std::map<std::string, std::string> data;
		for (auto a = n->first_attribute(); a != nullptr; a = a->next_attribute()) {
			data.insert({ a->name(), a->value() });
		}
		entityDatas.push_back(data);
		// Load the attributes from the children nodes (the components)
		std::vector<std::map<std::string, std::string>> cData;
		for (auto cNode = n->first_node("Component"); cNode != nullptr; cNode = cNode->next_sibling()) {
			std::map<std::string, std::string> cd;
			for (auto cAttr = cNode->first_attribute(); cAttr != nullptr; cAttr = cAttr->next_attribute()) {
				cd.insert({ cAttr->name(), cAttr->value() });
			}
			cData.push_back(cd);
		}
		componentDatas.push_back(cData);
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
		e->id = std::stoi(it["id"]);
		// Add to list
		entities.push_back(e);
		// Set to player if it is
		if (entities.back()->components.controller != nullptr && std::dynamic_pointer_cast<PlayerShipController>(entities.back()->components.controller)) {
			player = entities.back();
			player.lock()->components.inventory->addItems(GameResource::Gold, 300);
		}
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
		std::shared_ptr<Entity> e = g->getEntityById(std::stoi(entityDatas[i]["id"])).lock();
		std::vector<std::map<std::string, std::string>> cDatas = componentDatas[i];
		for (ComponentType c : ComponentList::allTypes) {
			std::map<std::string, std::string> data;
			for (auto cd : cDatas) {
				if (std::stoi(cd["type"]) == (int)(c)) {
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