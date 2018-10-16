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

Game::Game() {}
Game::Game(App* app): app(app)
{
	// Create world and make gravity 0, since it is top down
	this->world = std::shared_ptr<b2World>(new b2World({ 0.0f, 0.0f }));
	this->listener = SimpleCollisionListener();
	this->world->SetContactListener(&this->listener);
	// Create GameMap
	rapidxml::file<> file("test.xml");
	rapidxml::xml_document<> doc;
	doc.parse<0>(file.data());
	this->gMap = GameMap(this, &doc);
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
	this->gHud = GameHud(this);
	this->fpsText = tgui::TextBox::create();
	this->fpsText->setPosition({ this->app->getWindow()->getSize().x - 200.0f, 0.0f });
	this->app->getGui()->add(this->fpsText);

}

void Game::update(double delta)
{
	this->fpsText->setText(std::to_string(1.0 / delta));
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
			e->components.renderer->renderCollider(r);
		}
	}
	// Render debug info, i.e. hitboxes
	for (auto e : this->entities) {
		if (e->components.renderer != nullptr) {
			e->components.renderer->renderCollider(r);
		}
	}
	r.render(this->app->getWindow());
	r.reset();
	// Render GUI
	this->app->getGui()->draw();
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
tgui::Gui* Game::getGui() {
	return this->app->getGui();
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