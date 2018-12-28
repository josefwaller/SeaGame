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
	auto x = 0;
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
	auto eN = doc.first_node("EntityList");
	for (auto n = eN->first_node("Entity"); n != nullptr; n = n->next_sibling()) {
		std::map<std::string, std::string> data;
		for (auto a = n->first_attribute(); a != nullptr; a = a->next_attribute()) {
			data.insert({ a->name(), a->value() });
		}
		entityDatas.push_back(data);
	}
	// Vector of entities
	std::vector<std::shared_ptr<Entity>> entities;
	// The player, which is also in entieies
	std::weak_ptr<Entity> player;
	// Create the entities from the data
	for (auto it : entityDatas) {
		entities.push_back(EntityPrefabs::getEntityFromSaveData(g, it));
		// Set to player if it is
		if (entities.back()->components.controller != nullptr && std::dynamic_pointer_cast<PlayerShipController>(entities.back()->components.controller)) {
			player = entities.back();
			player.lock()->components.inventory->addItems(GameResource::Gold, 300);
		}
	}
	// Set into-component relationships, since now all the components have been created
	for (size_t i = 0; i < entityDatas.size(); i++) {
		for (ComponentType c : ComponentList::allTypes) {
			std::shared_ptr<Component> comp = entities[i]->components.get(c);
			if (comp) {
				comp->fromSaveData(entityDatas[i]);
			}
		}
	}
	// Add values to game
	g->loadFromData(
		gMap,
		entities,
		player,
		TechTree()
	);
	// Return the game
	return std::move(game);
}