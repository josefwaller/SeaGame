#include "Game.h"
#include <memory>
#include "Entity.h"
#include "EntityPrefabs.h"
#include "RenderManager.h"
#include "ShipRenderer.h"
#include "SimpleCollisionListener.h"
#include "PlayerShipController.h"
#include <memory>
#include <fstream>
#include <iostream>
#include <rapidxml\rapidxml_print.hpp>
#include <rapidxml\rapidxml_utils.hpp>
#include "App.h"

const float Game::BOX2D_TO_WORLD = 100.0f;
const float Game::WORLD_TO_BOX2D = 1 / Game::BOX2D_TO_WORLD;
Game::Game() {}
Game::~Game() {
	this->app->getGui()->remove(this->guiContainer);
}
Game::Game(App* app): app(app)
{
	// Create world and make gravity 0, since it is top down
	this->world = std::shared_ptr<b2World>(new b2World({ 0.0f, 0.0f }));
	this->listener = SimpleCollisionListener();
	this->world->SetContactListener(&this->listener);
	this->guiContainer = tgui::Group::create();
	this->fpsText = tgui::TextBox::create();
	this->fpsText->setPosition({ this->app->getWindow()->getSize().x - 200.0f, 0.0f });
	this->guiContainer->add(this->fpsText);
	this->app->getGui()->add(this->guiContainer);
	this->gHud = GameHud(this);
}
void Game::generateNew() {
	// Generate new map
	this->gMap = GameMap(this);
	// Start with the beginning amount of money
	this->money = Game::STARTING_MONEY;
	// Add the player in the first sea tile
	for (size_t x = 0; x < this->gMap.getMapSize().x; x++) {
		for (size_t y = 0; y < this->gMap.getMapSize().y; y++) {
			if (this->gMap.getTileAt(x, y) == GameMap::TileType::Sea) {
				// Add player
				this->addEntity(EntityPrefabs::playerShip(this, { x * 64.0f, y * 64.0f }));
				this->player = this->entities.back();
				// Todo: make a better algorithm than returning here
				return;
			}
		}
	}
}
void Game::loadFromFile(std::string fileName) {
	// Create GameMap
	rapidxml::file<> file(fileName.c_str());
	rapidxml::xml_document<> doc;
	doc.parse<0>(file.data());
	this->gMap = GameMap(this, &doc);
	// TBA: load money
	this->money = 200;
	std::vector<std::map<std::string, std::string>> entityDatas;
	auto eN = doc.first_node("EntityList");
	for (auto n = eN->first_node("Entity"); n != nullptr; n = n->next_sibling()) {
		std::map<std::string, std::string> data;
		for (auto a = n->first_attribute(); a != nullptr; a = a->next_attribute()) {
			data.insert({ a->name(), a->value() });
		}
		entityDatas.push_back(data);
	}
	for (auto it : entityDatas) {
		this->entities.push_back(EntityPrefabs::getEntityFromSaveData(this, it));
		if (this->entities.back()->components.controller != nullptr && std::dynamic_pointer_cast<PlayerShipController>(this->entities.back()->components.controller)) {
			this->player = this->entities.back();
			this->player.lock()->components.inventory->addItems(GameResource::Gold, 300);
		}
	}
	for (size_t i = 0; i < entityDatas.size(); i++) {
		for (ComponentType c : ComponentList::allTypes) {
			std::shared_ptr<Component> comp = this->entities[i]->components.get(c);
			if (comp) {
				comp->fromSaveData(entityDatas[i]);
			}
		}
	}

}

void Game::update(double delta)
{
	this->fpsText->setText(std::to_string(1.0 / delta));
	this->gHud.update();
	// Update all entities
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
#ifdef _DEBUG
		if (e->game != this)
			auto x = 0;
#endif
		if (e->components.controller != nullptr)
			e->components.controller->update((float)delta);
	}
	// Update world and resolve collisions
	this->world->Step((float)delta, 8, 3);
	// Remove entities
	for (auto it : this->toRemove) {
		this->entities.erase(
			std::remove(this->entities.begin(), this->entities.end(), it.lock()),
			this->entities.end());
	}
	this->toRemove = {};
#ifdef _DEBUG
	if (this->entities.size() != Entity::trueEntityCount) {
		// :(
		auto a = this->entities.size();
		auto b = Entity::trueEntityCount;
		auto c = 0;
	}
	for (auto it = this->entities.begin(); it != this->entities.end(); it++) {
		for (auto x = it + 1; x != this->entities.end(); x++) {
			if ((*it)->id == (*x)->id) {
				auto y = 0;
			}
		}
	}
#endif
}

void Game::render()
{
	// Render Entities
	RenderManager r;
	this->gMap.render(r);
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->components.renderer != nullptr) {
			e->components.renderer->render(r);
		}
	}
	// Render debug info, i.e. hitboxes
	for (auto e : this->entities) {
		if (e->components.renderer != nullptr) {
//			e->components.renderer->renderCollider(r);
		}
	}
	r.render(this->app->getWindow());
	r.reset();
}
void Game::save() {
	rapidxml::xml_document<> saveData;
	this->gMap.addSaveData(&saveData);
	auto n = saveData.allocate_node(rapidxml::node_element, "EntityList");
	for (auto e : this->entities) {
		auto data = e->getSaveData();
		auto eN = saveData.allocate_node(rapidxml::node_element, "Entity");
		for (auto it : data) {
			char* a = saveData.allocate_string(it.first.c_str());
			char* b = saveData.allocate_string(it.second.c_str());
			eN->append_attribute(
				saveData.allocate_attribute(
					a, b
				)
			);
		}
		n->append_node(eN);
	}
	saveData.append_node(n);
	std::ofstream f;
	f.open("test.xml");
	f << saveData;
	f.close();
}
void Game::handleEvent(sf::Event e) {
	switch (e.type) {
	case sf::Event::MouseButtonPressed:
		if (e.key.code == sf::Mouse::Left) {
			this->gHud.onClick(this->getMouseCoords());
		}
		break;
	}
}
std::vector<std::shared_ptr<Entity>> Game::getEntities()
{
	return this->entities;
}
std::weak_ptr<Entity> Game::getEntityById(unsigned long id) {
	for (auto it = this->entities.begin(); it != this->entities.end(); it++) {
		if ((*it)->id == id) {
			return *it;
		}
	}
	// Return nullptr if no entity has that id
	return std::weak_ptr<Entity>();
}
void Game::addEntity(std::shared_ptr<Entity> newEnt)
{
	this->entities.push_back(newEnt);
}
void Game::removeEntity(std::weak_ptr<Entity> e)
{
	this->toRemove.push_back(e);
}
sf::Vector2f Game::getMouseCoords()
{
	return (sf::Vector2f)sf::Mouse::getPosition(*this->app->getWindow());
}

std::shared_ptr<Entity> Game::getPlayer()
{
	return this->player.lock();
}

std::weak_ptr<b2World> Game::getWorld()
{
	return this->world;
}
tgui::Group::Ptr Game::getGui() {
	return this->guiContainer;
}
GameMap* Game::getGameMap() {
	return &this->gMap;
}
GameHud* Game::getHud() {
	return &this->gHud;
}
TechTree* Game::getTechTree() {
	return &this->techTree;
}
unsigned int Game::getMoney() {
	return this->money;
}
void Game::addMoney(unsigned int amount) {
	this->money += amount;
}
void Game::removeMoney(unsigned int amount) {
	this->money -= amount;
}

sf::RenderWindow* Game::getWindow() {
	return this->app->getWindow();
}