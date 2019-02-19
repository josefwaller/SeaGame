#include <memory>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <TGUI/Gui.hpp>

#include "Game.h"
#include "Box2D/Box2D.h"
#include "Entity.h"
#include "EntityPrefabs.h"
#include "RenderManager.h"
#include "ShipRenderer.h"
#include "SimpleCollisionListener.h"
#include "PlayerShipController.h"
#include "App.h"
#include "SaveFile.h"

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
	this->theme = tgui::Theme("../TGUI-0.8/themes/Black.txt");
	tgui::Theme::setDefault(&this->theme);
	this->view = app->getWindow()->getDefaultView();
	// Create a listener for the world
	this->listener = SimpleCollisionListener();
	this->world->SetContactListener(&this->listener);
	// Create gui container
	this->guiContainer = tgui::Group::create();
	// Add text displaying the FPS in the corner
	this->fpsText = tgui::TextBox::create();
	this->fpsText->setPosition({ this->app->getWindow()->getSize().x - 200.0f, 0.0f });
	this->guiContainer->add(this->fpsText);
	this->app->getGui()->add(this->guiContainer);
	// Create game hud
	this->gHud = GameHud(this);
}
// Set a bunch of data fields when loading the game
void Game::loadFromData(
	GameMap gm,
	std::vector<std::shared_ptr<Entity>> entities,
	std::weak_ptr<Entity> player,
	TechTree tree,
	unsigned int money) {
	// Set the data fields
	this->gMap = gm;
	this->entities = entities;
	this->player = player;
	this->techTree = tree;
	this->money = money;
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
}
void Game::setGameMap(GameMap gm) {
	this->gMap = gm;
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
		if (e->components.controller != nullptr) {
			e->components.controller->update((float)delta);
		}
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
}
void Game::quitGame() {
	this->app->quitGame();
}
void Game::render()
{
	// Render Map (give direct access to window for speed reasons)
	this->gMap.render(this->getWindow());
	// Render Entities
	RenderManager r;
	for (size_t i = 0; i < this->entities.size(); i++) {
		auto e = this->entities[i];
		if (e->components.renderer != nullptr) {
			e->components.renderer->render(r);
		}
	}
	// Render HUD that isn't part of tgui
	this->gHud.render(r);
	// Render debug info, i.e. hitboxes
	for (auto e : this->entities) {
		if (e->components.renderer != nullptr) {
			// Make sure the entity is in the window before rendering
			if (e->components.transform) {
				sf::FloatRect rect = this->getViewRect();
				sf::Vector2f pos = e->components.transform->getPosition();
				if (!rect.contains(pos)) {
					continue;
				}
			}
			e->components.renderer->renderCollider(r);
		}
	}
	// Set view
	this->view.setCenter(this->player.lock()->components.transform->getPosition());
	this->app->getWindow()->setView(this->view);
	r.render(this->app->getWindow());
	r.reset();
}
void Game::save() {
	// Get date for file name
	auto time = std::chrono::system_clock::now();
	auto t = std::chrono::system_clock::to_time_t(time);
	// Get formatted date as string
	std::tm buf;
	localtime_s(&buf, &t);
	std::stringstream ss;
	ss << std::put_time(&buf, "%Y-%m-%d");
	std::string date = ss.str();
	// Save file
	SaveFile sf(this);
	sf.save(date);
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
	return this->getWindow()->mapPixelToCoords(
		sf::Mouse::getPosition(*this->app->getWindow())
	);
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
sf::View* Game::getView() {
	return &this->view;
}
sf::FloatRect Game::getViewRect() {
	return sf::FloatRect(this->view.getCenter() - this->view.getSize() / 2.0f, this->view.getSize());
}