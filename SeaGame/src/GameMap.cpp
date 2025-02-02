#include "GameMap.h"
#include "Game.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Box2dTransform.h"
#include "EntityPrefabs.h"
#include "SaveData.h"
#include <PerlinNoise.hpp>
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <random>

GameMap::GameMap(Game* g) : game(g)
{}
void GameMap::generate() {
	const unsigned int WIDTH = 30;
	const unsigned int HEIGHT = 30;
	// The width of the water border around the edges
	const unsigned int BORDER_WIDTH = 3;
	const float FREQUENCY = 25.0f;
	std::vector<std::vector<double>> noiseGrid = GameMap::getNoiseMap(
		WIDTH - 2 * BORDER_WIDTH,
		HEIGHT - 2 * BORDER_WIDTH,
		FREQUENCY,
		3);
	// Fill grid with sea
	this->tiles.resize(WIDTH);
	for (auto it = this->tiles.begin(); it != this->tiles.end(); it++) {
		it->resize(HEIGHT, Tile(TileType::Sea));
	}
	// Add land tiles
	// Build a map of where the resource deposits are to ensure cities aren't built on top of them
	for (auto x = BORDER_WIDTH; x < WIDTH - BORDER_WIDTH; x++) {
		for (auto y = BORDER_WIDTH; y < HEIGHT - BORDER_WIDTH; y++) {
			// Add land
			if (noiseGrid[x - BORDER_WIDTH][y - BORDER_WIDTH] > 0.5) {
				// Add a land tile
				this->addLandTile(x, y);
				// Temporarily, have a 5% chance to add a resource to it
				// Eventually will make actually spawn randomly
			}
		}
	}
	// Add Resources
	noiseGrid = getNoiseMap(
		(WIDTH - 2 * BORDER_WIDTH),
		(HEIGHT - 2 * BORDER_WIDTH),
		15.0f,
		0);
	// Temporary resource deck for shuffling before converting to a queue
	std::vector<GameResource> tempResDeck;
	for (size_t r = 0; r < 100; r++) {
		// 50% chance to spawn wood
		if (r < 50)
			tempResDeck.push_back(GameResource::Wood);
		// 20% for stone
		else if (r < 70)
			tempResDeck.push_back(GameResource::Stone);
		// 15% for copper
		else if (r < 85)
			tempResDeck.push_back(GameResource::Copper);
		// 10% for iron
		else if (r < 95)
			tempResDeck.push_back(GameResource::Iron);
		// 5% for gold
		else
			tempResDeck.push_back(GameResource::Gold);
	}
	auto rng = std::default_random_engine();
	std::shuffle(tempResDeck.begin(), tempResDeck.end(), rng);
	std::queue<GameResource> resourceDeck;
	for (GameResource r : tempResDeck) {
		resourceDeck.push(r);
	}
	for (size_t x = 0; x < noiseGrid.size(); x++) {
		for (size_t y = 0; y < noiseGrid[0].size(); y++) {
			if (noiseGrid[x][y] > 0.60 && this->getTileAt(x, y) == TileType::Land) {
				int r = rand() % 100;
				// Get resource and add to back of deck
				GameResource res = resourceDeck.front();
				resourceDeck.pop();
				tempResDeck.push_back(res);
				std::shared_ptr<Entity> e = EntityPrefabs::resourceSource(
					this->game,
					sf::Vector2f((float)x, (float)y) * 64.0f,
					res
				);
				this->game->addEntity(e);
				this->tiles[x][y].entity = e;
			}
		}
	}
	// Add cities
	const size_t CITIES_VERT = 1;
	const size_t CITIES_HORZ = 1;
	const size_t w = this->tiles.size() / CITIES_HORZ;
	const size_t h = this->tiles[0].size() / CITIES_VERT;
	for (size_t x = 0; x < CITIES_HORZ; x++) {
		for (size_t y = 0; y < CITIES_VERT; y++) {
			size_t minX = x * w;
			size_t minY = y * h;
			size_t maxX = (x + 1) * w;
			size_t maxY = (y + 1) * h;
			// Every 5th citie should instead be a pirate base
			if (rand() % 5 == 0) {
				addBuilding(EntityType::PirateFortress, minX, minY, maxX, maxY);
				addBuilding(EntityType::PirateBase, minX, minY, maxX, maxY);
				addBuilding(EntityType::PirateBase, minX, minY, maxX, maxY);
				addBuilding(EntityType::PirateBase, minX, minY, maxX, maxY);
			}
			else {
				// Currently don't do anything if building the city fails
				addBuilding(EntityType::City, minX, minY, maxX, maxY);
			}
		}
	}
	this->initTileRenderData();
}
std::vector<std::vector<double>> GameMap::getNoiseMap(int w, int h, float f, int sC) {
	siv::PerlinNoise noise((uint32_t)time(0));
	std::vector<std::vector<double>> noiseGrid;
	// Generate noise grid
	for (size_t x = 0; x < w; x++) {
		noiseGrid.push_back({});
		for (size_t y = 0; y < h; y++) {
			const float fx = (float)w / f;
			const float fy = (float)h / f;
			noiseGrid[x].push_back(noise.octaveNoise0_1(x / fx, y / fy, 16));
		}
	}
	// Smooth noise grid
	for (size_t _ = 0; _ < sC; _++) {
		std::vector<std::vector<double>> newNoiseGrid;
		for (size_t x = 0; x < w; x++) {
			newNoiseGrid.push_back({});
			for (size_t y = 0; y < h; y++) {
				double total = 0;
				unsigned int count = 0;
				for (int xOff = -1; xOff <= 1; xOff++) {
					if (x + xOff < w && x + xOff >= 0) {
						for (int yOff = -1; yOff <= 1; yOff++) {
							if (y + yOff < h && y + yOff >= 0) {
								total += noiseGrid[x + xOff][y + yOff];
								count++;
							}
						}
					}
				}
				newNoiseGrid[x].push_back(total / (double)count);
			}
		}
		noiseGrid = newNoiseGrid;
	}
	return noiseGrid;
}
void GameMap::loadFromData(SaveData data) {
	int w = std::stoi(data.getValue("width"));
	int h = std::stoi(data.getValue("height"));
	this->tiles.resize(w);
	for (auto it = this->tiles.begin(); it != this->tiles.end(); it++) {
		it->resize(h);
	}
	for (SaveData tile : data.getDatas()) {
		int x = std::stoi(tile.getValue("x"));
		int y = std::stoi(tile.getValue("y"));
		TileType t = (TileType)(std::stoi(tile.getValue("type")));
		if (t == TileType::Land) {
			this->addLandTile(x, y);
		}
	}
	this->initTileRenderData();
}
void GameMap::initTileEntities(SaveData data) {
	for (SaveData tile : data.getDatas()) {
		int x = std::stoi(tile.getValue("x"));
		int y = std::stoi(tile.getValue("y"));
		int eId = std::stoi(tile.getValue("entity"));
		if (eId != -1)
			this->setTileEntity(x, y, this->game->getEntityById(eId));
	}

}
void GameMap::addLandTile(size_t x, size_t y) {
	// Set the tile to land
	this->tiles[x][y] = TileType::Land;
	// Add collider
	b2PolygonShape box;
	box.SetAsBox(32 * Game::WORLD_TO_BOX2D, 32 * Game::WORLD_TO_BOX2D);
	b2FixtureDef fix;
	fix.shape = &box;
	b2BodyDef bodyDef;
	bodyDef.position.Set((32 + x * 64.0f) * Game::WORLD_TO_BOX2D, (32 + y * 64.0f) * Game::WORLD_TO_BOX2D);
	bodyDef.type = b2_staticBody;
	b2Body* body = this->game->getWorld().lock()->CreateBody(&bodyDef);
	body->CreateFixture(&fix);
	this->bodies.push_back(body);
}
bool GameMap::addBuilding(
	EntityType type,
	size_t startX,
	size_t startY,
	size_t endX,
	size_t endY) {

	for (size_t x = startX; x < endX - 3; x++) {
		for (size_t y = startY; y < endY - 3; y++) {
			// Check if a city can be built here
			bool canBuildCity = true;
			for (size_t xOff = 0; xOff < 3; xOff++) {
				for (size_t yOff = 0; yOff < 3; yOff++) {
					Tile t = this->tiles[x + xOff][y + yOff];
					if (t.type != TileType::Land || this->getTileIsFull(x + xOff, y + yOff)) {
						canBuildCity = false;
						break;
					}
				}
			}
			if (canBuildCity) {
				std::shared_ptr<Entity> e = EntityPrefabs::getEntityFromType(
					this->game,
					{ x * 64.0f, y * 64.0f },
					type
				);
				this->game->addEntity(e);
				for (size_t xOff = 0; xOff < 3; xOff++) {
					for (size_t yOff = 0; yOff < 3; yOff++) {
						this->setTileEntity(x + xOff, y + yOff, e);
					}
				}
				return true;
			}
		}
	}
	return false;
}
// Set the tile render data for each tile
void GameMap::initTileRenderData() {
	for (size_t x = 0; x < this->tiles.size(); x++) {
		for (size_t y = 0; y < this->tiles[x].size(); y++) {
			this->setTileRenderData(x, y);
		}
	}
}
bool GameMap::getTileIsFull(size_t x, size_t y) {
	if (!coordIsOnMap(x, y)) {
		return false;
	}
	return this->tiles[x][y].entity.lock() != nullptr;
}
void GameMap::setTileEntity(size_t x, size_t y, std::weak_ptr<Entity> e) {
	if (coordIsOnMap(x, y)) {
		this->tiles[x][y].entity = e;
	}
}
std::weak_ptr<Entity> GameMap::getTileEntity(size_t x, size_t y) {
	if (coordIsOnMap(x, y)) {
		return this->tiles[x][y].entity;
	}
	return std::weak_ptr<Entity>();
}
bool GameMap::coordIsOnMap(size_t x, size_t y) {
	if (x < 0 || x >= this->tiles.size() || y < 0 || y >= this->tiles[0].size()) {
		return false;
	}
	return true;
}
void GameMap::render(sf::RenderWindow* window)
{
	auto c = this->game->getView()->getCenter();
	auto s = this->game->getView()->getSize();
	// Render the tiles
	int minX = (int)floor((c.x - s.x / 2) / 64.0f);
	int minY = (int)floor((c.y - s.y / 2) / 64.0f);
	int maxX = (int)ceil((c.x + s.x / 2) / 64.0f);
	int maxY = (int)ceil((c.y + s.y / 2) / 64.0f);
	sf::Sprite spr = ResourceManager::get()->getSprite("tiles", "sea-sea-sea-sea", false);
	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			spr.setPosition(x * 64.0f, y * 64.0f);
			window->draw(spr);
			if (x >= 0 && y >= 0) {
				this->drawTile(window, x, y);
			}
		}
	}
}
void GameMap::renderDebug(sf::RenderWindow* window) {
	// Draw collision bodies
	sf::FloatRect rect = this->game->getViewRect();
	for (auto b_it : this->bodies) {
		if (!rect.contains(
			sf::Vector2f(b_it->GetPosition().x, b_it->GetPosition().y) * Game::BOX2D_TO_WORLD)) {
			continue;
		}
		for (b2Fixture* fix = b_it->GetFixtureList(); fix; fix = fix->GetNext()) {
			// Get shape
			b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(fix->GetShape());
			// Create drawable vertice array
			sf::VertexArray drawable(sf::LineStrip, shape->m_count + 1);
			// Add all points
			for (int i = 0; i < shape->m_count; i++) {
				drawable[i].position = {
					(b_it->GetPosition().x + shape->m_vertices[i].x) * Game::BOX2D_TO_WORLD,
					(b_it->GetPosition().y + shape->m_vertices[i].y) * Game::BOX2D_TO_WORLD
				};
				drawable[i].color = sf::Color::Red;
			}
			// Make the debug render a closed loop
			drawable[shape->m_count] = drawable[0];
			// Render
			window->draw(drawable);
		}
	}
	// Render tile debug info
	auto c = window->getView().getCenter();
	auto s = window->getView().getSize();
	// Render the tiles
	int minX = std::max((int)floor((c.x - s.x / 2) / 64.0f), 0);
	int minY = std::max((int)floor((c.y - s.y / 2) / 64.0f), 0);
	int maxX = std::min((int)ceil((c.x + s.x / 2) / 64.0f), (int)this->tiles.size());
	int maxY = std::min((int)ceil((c.y + s.y / 2) / 64.0f), (int)this->tiles[0].size());

	for (size_t x = minX; x < maxX; x++) {
		for (size_t y = minY; y < maxY; y++) {
			this->tiles[x][y].renderDebug((float)x * 64.0f, (float)y * 64.0f, window);
		}
	}
}
void GameMap::drawTile(sf::RenderWindow* window, size_t x, size_t y) {
	// Make sure the tile is on the map
	if (!coordIsOnMap(x, y)) {
		return;
	}
	// Get tile and make sure it's land
	Tile tile = this->tiles[x][y];
	if (tile.type == TileType::Sea) {
		return;
	}
	tile.render((float)x, (float)y, window);
}
SaveData GameMap::getSaveData() {
	SaveData s("GameMap");
	s.addValue("width", std::to_string(this->getMapSize().x));
	s.addValue("height", std::to_string(this->getMapSize().y));
	for (size_t x = 0; x < this->tiles.size(); x++) {
		for (size_t y = 0; y < this->tiles[x].size(); y++) {
			SaveData t("GameMapTile");
			t.addValue("x", std::to_string(x));
			t.addValue("y", std::to_string(y));
			t.addValue("type", std::to_string(this->tiles[x][y].type));
			// Get either the entity id, or -1 if it is empty
			int eId = -1;
			if (this->tiles[x][y].entity.lock()) {
				eId = this->tiles[x][y].entity.lock()->id;
			}
			t.addValue("entity", std::to_string(eId));
			s.addData(t);
		}
	}
	return s;
}
TileType GameMap::getTileAt(size_t x, size_t y) {
	if (x >= 0 && x < this->tiles.size()) {
		if (y >= 0 && y < this->tiles[0].size()) {
			return this->tiles[x][y].type;
		}
	}
	return TileType::Sea;
}
sf::Vector2<size_t> GameMap::getMapSize() {
	return { this->tiles.size(), this->tiles[0].size() };
}
void GameMap::setTileRenderData(size_t x, size_t y) {
	// Get the sprites
	Tile* t = &this->tiles[x][y];
	t->data = Tile::getTileRenderData(
		this->getTileAt(x, y - 1),
		this->getTileAt(x - 1, y),
		this->getTileAt(x, y + 1),
		this->getTileAt(x + 1, y),
		t->type
	);
}
