#include "AutomatedShipController.h"
#include <map>
#include <math.h>

void AutomatedShipController::move(float delta) {
	auto trans = this->getParent().lock()->components.transform;
	sf::Vector2f difference = points[pointsIndex]  + sf::Vector2f(32.0f, 32.0f) - trans->getPosition();
	float angle = atan2(difference.y, difference.x);
	trans->setRotation(angle);
	if (sqrt(difference.x * difference.x + difference.y * difference.y) > 64) {
		this->accelerate();
	}
	else {
		this->pointsIndex++;
		if (this->pointsIndex >= this->points.size()) {
			this->onReachingTarget();
			this->pointsIndex = 0;
		}
	}
}
void AutomatedShipController::setTarget(sf::Vector2f target) {
	// Ensure the target is on water
	sf::Vector2<size_t> targetCoord = sf::Vector2<size_t>(target / 64.0f);
	// Get the target the ship is on
	sf::Vector2<size_t> startCoord = sf::Vector2<size_t>(this->getParent().lock()->components.transform->getPosition() / 64.0f);
	// Build a list of tiles to go through
	std::vector<sf::Vector2<size_t>> coords;
	// The tiles that have been visited and/or are in the queue
	// i.e. don't add these tiles
	std::vector<sf::Vector2<size_t>> visited;
	// Add the  starting coord to the coords
	coords.push_back(startCoord);
	visited.push_back(startCoord);
	// The "steps" each tile has
	// Basically the number of tiles away from the source, and the previous tile
	// to go to directly before this one
	std::map<size_t, std::map<size_t, std::pair<sf::Vector2<size_t>, unsigned int>>> steps;
	steps[startCoord.x][startCoord.y] = { {0, 0}, 0 };
	// Get game map for easy reference
	GameMap* gMap = this->getParent().lock()->game->getGameMap();
	while (true) {
		if (coords.size() == 0)
			break;
		// Get the coord with the minimum score
		sf::Vector2<size_t> c = *(coords.begin());
		auto cIt = coords.begin();
		for (auto it = coords.begin(); it != coords.end(); it++) {
			// Check if it has the minimum distance to the target
			// Checks the one with the minimum distance first
			if (pow(it->x - targetCoord.x, 2) + pow(it->y - targetCoord.y, 2) < pow(c.x - targetCoord.x, 2) + pow(c.y - targetCoord.y, 2)) {
				c = *it;
				cIt = it;
			}
		}
		coords.erase(cIt);
		// Check if the coord is the target
		if (c == targetCoord) {
			std::vector<sf::Vector2f> trail;
			trail.push_back(sf::Vector2f(c) * 64.0f);
			while (true) {
				sf::Vector2<size_t> next = steps[c.x][c.y].first;
				if (next.x == 0 && next.y == 0) {
					break;
				}
				trail.push_back(sf::Vector2f(next.x * 64.0f, next.y * 64.0f));
				c = next;
			}
			std::reverse(trail.begin(), trail.end());
			this->points = trail;
			break;
		}
		else {
			// Add the coords around it
			std::vector<sf::Vector2<size_t>> toAdd;
			for (auto xOff : { -1, 0, 1 }) {
				for (auto yOff : { -1, 0, 1 }) {
					// Don't add the coord it's on
					if (xOff == 0 && yOff == 0) continue;
					// Check the tile exists and is sea
					if ((int)c.x + xOff >= 0 && (int)c.x + xOff < gMap->getMapSize().x) {
						if ((int)c.y + yOff >= 0 && (int)c.y + yOff < gMap->getMapSize().y) {
							if (gMap->getTileAt(c.x + xOff, c.y + yOff) == GameMap::TileType::Sea) {
								// Add the tile
								toAdd.push_back(sf::Vector2<size_t>(c.x + xOff, c.y + yOff));
							}
						}
					}
				}
			}
			for (auto it : toAdd) {
				if (std::find(visited.begin(), visited.end(), it) == visited.end()) {
					coords.push_back(it);
					visited.push_back(it);
					steps[it.x][it.y] = { c, steps[c.x][c.y].second + 1 };
				}
			}
		}
	}
}
// Empty, may be filled in by subclass
void AutomatedShipController::onReachingTarget() {

}