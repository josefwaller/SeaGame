#include "BaseController.h"
#include "GameMap.h"

void BaseController::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
}

sf::Vector2f BaseController::getDockCoords() {
	// Set the dock coords
	sf::Vector2f pos = this->getParent().lock()->components.transform->getPosition();
	std::queue<sf::Vector2i> coords;
	std::vector<sf::Vector2i> visited;
	coords.push((sf::Vector2i)(pos / 64.0f));
	GameMap* gMap = this->getParent().lock()->game->getGameMap();
	while (true) {
		// Get the coords
		sf::Vector2i c = coords.front();
		coords.pop();
		// Check if it is sea
		if (gMap->getTileAt(c.x, c.y) == GameMap::TileType::Sea) {
			this->dockCoords = (sf::Vector2f)(c) * 64.0f;
			break;
		}
		else {
			// Add the surrounding tiles
			for (auto off : { -1, 1 }) {
				std::vector<sf::Vector2i> potentialCoords = {
					c + sf::Vector2i(off, 0),
					c + sf::Vector2i(0, off)
				};
				for (auto pC : potentialCoords) {
					if (pC.x < gMap->getMapSize().x && pC.x >= 0) {
						if (pC.y < gMap->getMapSize().y && pC.y >= 0) {
							// Make sure it hasn't checked this point before
							if (std::find(visited.begin(), visited.end(), pC) == visited.end()) {
								coords.push(pC);
							}
						}
					}
				}
			}
			// Add the tile to visted
			visited.push_back(c);
		}
	}

	return this->dockCoords;
}