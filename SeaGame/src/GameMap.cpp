#include <queue>
#include "GameMap.h"
#include "ResourceManager.h"
#include "Box2dTransform.h"
#include <PerlinNoise.hpp>
#include <ctime>
#include <cstdio>

GameMap::GameMap() {}
GameMap::GameMap(Game* g): game(g)
{
	const unsigned int WIDTH = 32;
	const unsigned int HEIGHT = 17;
	const float frequency = 25.0f;
	siv::PerlinNoise noise((uint32_t)time(0));
	std::vector<std::vector<double>> noiseGrid;
	// Generate noise grid
	for (size_t x = 0; x < WIDTH; x++) {
		noiseGrid.push_back({});
		for (size_t y = 0; y < HEIGHT; y++) {
			const float fx = WIDTH / frequency;
			const float fy = HEIGHT / frequency;
noiseGrid[x].push_back(noise.octaveNoise0_1(x / fx, y / fy, 16));
		}
	}
	// Smooth noise grid
	for (size_t _ = 0; _ < 3; _++) {
		std::vector<std::vector<double>> newNoiseGrid;
		for (size_t x = 0; x < WIDTH; x++) {
			newNoiseGrid.push_back({});
			for (size_t y = 0; y < HEIGHT; y++) {
				double total = 0;
				unsigned int count = 0;
				for (int xOff = -1; xOff <= 1; xOff++) {
					if (x + xOff < WIDTH && x + xOff >= 0) {
						for (int yOff = -1; yOff <= 1; yOff++) {
							if (y + yOff < HEIGHT && y + yOff >= 0) {
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
	for (auto x = 0; x < WIDTH; x++) {
		for (auto y = 0; y < HEIGHT; y++) {
			// Add test island in the middle
			if (noiseGrid[x][y] > 0.5) {
				// Add a land tile
				this->addLandTile(x, y);
			}
		}
	}
	resetTexture();
}
GameMap::GameMap(Game* g, rapidxml::xml_document<>* doc): game(g) {

	auto gMapNode = doc->first_node("GameMap");
	size_t w = std::stoi(gMapNode->first_attribute("width")->value());
	size_t h = std::stoi(gMapNode->first_attribute("height")->value());
	this->tiles.resize(w);
	// Fill tiles with sea
	for (auto it = this->tiles.begin(); it != this->tiles.end(); ++it) {
		it->resize(h, TileType::Sea);
	}
	for (auto n = gMapNode->first_node(); n != nullptr; n = n->next_sibling()) {
		size_t x = (size_t)std::stoi(n->first_attribute("x")->value());
		size_t y = (size_t)std::stoi(n->first_attribute("y")->value());
		std::string valStr = std::string(n->first_attribute("type")->value());
		TileType val = valStr == std::string("Land") ? TileType::Land : TileType::Sea;
		if (val == TileType::Land) {
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