#include <queue>
#include "GameMap.h"
#include "ResourceManager.h"
#include "Box2dTransform.h"
#include "EntityPrefabs.h"
#include <PerlinNoise.hpp>
#include <ctime>
#include <cstdio>

GameMap::GameMap() {}
GameMap::GameMap(Game* g): game(g)
{
	const unsigned int WIDTH = 100;
	const unsigned int HEIGHT = 100;
	// The width of the water border around the edges
	const unsigned int BORDER_WIDTH = 3;
	const float frequency = 25.0f;
	siv::PerlinNoise noise((uint32_t)time(0));
	std::vector<std::vector<double>> noiseGrid;
	// Generate noise grid
	for (size_t x = 0; x < WIDTH - 2 * BORDER_WIDTH; x++) {
		noiseGrid.push_back({});
		for (size_t y = 0; y < HEIGHT - 2 * BORDER_WIDTH; y++) {
			const float fx = WIDTH / frequency;
			const float fy = HEIGHT / frequency;
noiseGrid[x].push_back(noise.octaveNoise0_1(x / fx, y / fy, 16));
		}
	}
	// Smooth noise grid
	for (size_t _ = 0; _ < 3; _++) {
		std::vector<std::vector<double>> newNoiseGrid;
		for (size_t x = 0; x < WIDTH - 2 * BORDER_WIDTH; x++) {
			newNoiseGrid.push_back({});
			for (size_t y = 0; y < HEIGHT - 2 * BORDER_WIDTH; y++) {
				double total = 0;
				unsigned int count = 0;
				for (int xOff = -1; xOff <= 1; xOff++) {
					if (x + xOff < WIDTH - 2 * BORDER_WIDTH && x + xOff >= 0) {
						for (int yOff = -1; yOff <= 1; yOff++) {
							if (y + yOff < HEIGHT - 2 * BORDER_WIDTH && y + yOff >= 0) {
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
	// Fill grid with sea
	this->tiles.resize(WIDTH);
	for (auto it = this->tiles.begin(); it != this->tiles.end(); it++) {
		it->resize(HEIGHT, TileType::Sea);
	}
	// Add land tiles
	// Build a map of where the resource deposits are to ensure cities aren't built on top of them
	std::vector<sf::Vector2i> resourcePositions;
	for (auto x = BORDER_WIDTH; x < WIDTH - BORDER_WIDTH; x++) {
		for (auto y = BORDER_WIDTH; y < HEIGHT - BORDER_WIDTH; y++) {
			// Add land
			if (noiseGrid[x - BORDER_WIDTH][y - BORDER_WIDTH] > 0.5) {
				// Add a land tile
				this->addLandTile(x, y);
				// Temporarily, have a 5% chance to add a resource to it
				// Eventually will make actually spawn randomly
				int r = rand() % 100;
				if (r < 6) {
					GameResource res;
					switch (rand() % 5) {
					case 0:
						res = GameResource::Wood;
						break;
					case 1:
						res = GameResource::Stone;
						break;
					case 2:
						res = GameResource::Copper;
						break;
					case 3:
						res = GameResource::Iron;
						break;
					case 4:
						res = GameResource::Gold;
						break;
					}
					this->game->addEntity(
						EntityPrefabs::resourceSource(this->game, sf::Vector2i((int)x * 64, (int)y * 64), res)
					);
					resourcePositions.push_back(sf::Vector2i(x, y));
				}
			}
		}
	}
	// Add cities
	const size_t CITIES_VERT = 2;
	const size_t CITIES_HORZ = 2;
	const size_t w = this->tiles.size() / CITIES_HORZ;
	const size_t h = this->tiles[0].size() / CITIES_VERT;
	for (size_t x = 0; x < CITIES_HORZ; x++) {
		for (size_t y = 0; y < CITIES_VERT; y++) {
			// Currently don't do anything if building the city fails
			addCity(x * w, y * h, (x + 1) * w, (y + 1) * h, resourcePositions);
		}
	}
	resetTexture();
}
GameMap::GameMap(Game* g, SaveData data): game(g) {
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
	resetTexture();
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
bool GameMap::addCity(size_t startX, size_t startY, size_t endX, size_t endY,
	std::vector<sf::Vector2i> resPos) {
	for (size_t x = startX; x < endX - 3; x++) {
		for (size_t y = startY; y < endY - 3; y++) {
			// Check if a city can be built here
			bool canBuildCity = true;
			for (size_t xOff = 0; xOff < 3; xOff++) {
				for (size_t yOff = 0; yOff < 3; yOff++) {
					if (this->tiles[x + xOff][y + yOff] != TileType::Land
						|| std::find(resPos.begin(), resPos.end(), sf::Vector2i(x + xOff, y + yOff)) != resPos.end()) {
						canBuildCity = false;
						break;
					}
				}
			}
			if (canBuildCity) {
				this->game->addEntity(EntityPrefabs::city(this->game, sf::Vector2i(x, y) * 64));
				return true;
			}
		}
	}
	return false;
}
void GameMap::resetTexture() {
	sf::RenderTexture rt;
	sf::Vector2<size_t> size = this->getMapSize();
	rt.create(64 * (unsigned int)size.x, 64 * (unsigned int)size.y);
	// Render a 10x10 grid of sea tiles
	for (size_t x = 0; x < this->tiles.size(); x++) {
		for (size_t y = 0; y < this->tiles[x].size(); y++) {
			// Draw water behind
			sf::Sprite s = ResourceManager::get()->getSprite("tiles", "sea-sea-sea-sea", false);
			s.setPosition(64.0f * x, 64.0f * y);
			rt.draw(s);
			this->drawTile(&rt, x, y);
		}
	}
	rt.display();
	this->texture = rt.getTexture();

}
void GameMap::render(RenderManager& r)
{
	sf::Sprite s;
	s.setTexture(this->texture);
	r.add(s, RenderManager::INDEX_LAND_TILES);
	// Draw collision bodies
	for (auto b_it : this->bodies) {
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
			r.add(drawable, RenderManager::INDEX_DEBUG);
		}
	}

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
			t.addValue("type", std::to_string(this->tiles[x][y]));
			s.addData(t);
		}
	}
	return s;
}
void GameMap::addSaveData(rapidxml::xml_document<>* doc) {
	rapidxml::xml_node<>* n = doc->allocate_node(rapidxml::node_element, "GameMap");
	char* w = doc->allocate_string(std::to_string(this->getMapSize().x).c_str());
	char* h = doc->allocate_string(std::to_string(this->getMapSize().y).c_str());
	n->append_attribute(doc->allocate_attribute("width", w));
	n->append_attribute(doc->allocate_attribute("height", h));
	for (size_t x = 0; x < this->getMapSize().x; x++) {
		for (size_t y = 0; y < this->getMapSize().y; y++) {
			auto tileNode = doc->allocate_node(rapidxml::node_element, "Tile");
			char* xStr = doc->allocate_string(std::to_string(x).c_str());
			char* yStr = doc->allocate_string(std::to_string(y).c_str());
			char* type = doc->allocate_string(
				this->tiles[x][y] == TileType::Sea ? "Sea" : "Land"
			);
			tileNode->append_attribute(doc->allocate_attribute("x", xStr));
			tileNode->append_attribute(doc->allocate_attribute("y", yStr));
			tileNode->append_attribute(doc->allocate_attribute("type", type));
			n->append_node(tileNode);
		}
	}
	doc->append_node(n);
}
GameMap::TileType GameMap::getTileAt(size_t x, size_t y) {
	if (x >= 0 && x < this->tiles.size()) {
		if (y >= 0 && y < this->tiles[0].size()) {
			return this->tiles[x][y];
		}
	}
	return TileType::Sea;
}
sf::Vector2<size_t> GameMap::getMapSize() {
	return { this->tiles.size(), this->tiles[0].size() };
}
void GameMap::drawTile(sf::RenderTexture* rt, size_t x, size_t y) {
	if (this->tiles[x][y] == TileType::Sea) {
		return;
	}
	TileType top, bottom, left, right, center;
	if (x == 0) {
		left = TileType::Sea;
	}
	else {
		left = this->tiles[x - 1][y];
	}
	if (x == this->tiles.size() - 1) {
		right = TileType::Sea;
	}
	else {
		right = this->tiles[x + 1][y];
	}
	if (y == 0) {
		top = TileType::Sea;
	}
	else {
		top = this->tiles[x][y - 1];
	}
	if (y == this->tiles[x].size() - 1) {
		bottom = TileType::Sea;
	}
	else {
		bottom = this->tiles[x][y + 1];
	}
	center = this->tiles[x][y];
	// Get the sprites
	sf::Sprite topLeftSprite, bottomLeftSprite, topRightSprite, bottomRightSprite;
	topLeftSprite = getSprite(top, center, center, left);
	topRightSprite = getSprite(top, right, center, center);
	bottomRightSprite = getSprite(center, right, bottom, center);
	bottomLeftSprite = getSprite(center, center, bottom, left);
	std::vector<sf::Sprite> sprites = { topLeftSprite, topRightSprite, bottomLeftSprite, bottomRightSprite };
	for (size_t i = 0; i < sprites.size(); i++) {
		sprites[i].setScale(0.5f, 0.5f);
		float pY = 64.0f * y;
		if (i >= 2) {
			pY += 32.0f;
		}
		sprites[i].setPosition(64.0f * x + (i % 2) * 32.0f, pY);
		rt->draw(sprites[i]);
	}
}
sf::Sprite GameMap::getSprite(TileType top, TileType right, TileType bottom, TileType left) {
	return ResourceManager::get()->getSprite("tiles",
		getTileString(top) + "-" +
		getTileString(right) + "-" +
		getTileString(bottom) + "-" +
		getTileString(left),
	false);
}
std::string GameMap::getTileString(TileType t) {
	if (t == TileType::Land) {
		return "land";
	}
	else {
		return "sea";
	}
}