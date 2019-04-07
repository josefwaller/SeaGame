#include "AutomatedShipController.h"
#include <map>
#include <math.h>
#include <memory>
#include "BaseController.h"
#include "Entity.h"

void AutomatedShipController::move(float delta, float speed) {
	// Check if the entity has already arrived
	if (this->pointsIndex >= this->points.size()) {
		this->onReachingTarget();
		this->pointsIndex = 0;
	}
	else {
		// Move
		auto trans = this->getComponentList().transform;
		sf::Vector2f difference = points[pointsIndex]  + sf::Vector2f(32.0f, 32.0f) - trans->getPosition();
		float angle = atan2(difference.y, difference.x);
		trans->setRotation(angle);
		if (sqrt(difference.x * difference.x + difference.y * difference.y) > 64) {
			this->accelerate(speed);
		}
		else {
			this->pointsIndex++;
		}
	}
}
void AutomatedShipController::setTarget(sf::Vector2f target) {
	// Reset index
	this->pointsIndex = 0;
	// Ensure the target is on water
	sf::Vector2i targetCoord = sf::Vector2i(target / 64.0f);
	// Get the target the ship is on
	sf::Vector2i startCoord = sf::Vector2i(this->getComponentList().transform->getPosition() / 64.0f);
	// Build a list of tiles to go through
	std::vector<sf::Vector2i> coords;
	// The tiles that have been visited and/or are in the queue
	// i.e. don't add these tiles
	std::vector<sf::Vector2i> visited;
	// Add the  starting coord to the coords
	coords.push_back(startCoord);
	visited.push_back(startCoord);
	// The "steps" each tile has
	// Basically the number of tiles away from the source, and the previous tile
	// to go to directly before this one
	std::map<size_t, std::map<size_t, std::pair<sf::Vector2i, unsigned int>>> steps;
	steps[startCoord.x][startCoord.y] = { {0, 0}, 0 };
	// Get game map for easy reference
	GameMap* gMap = this->getGame()->getGameMap();
	// Only check the 10 000 most likely tiles
	unsigned int count = 0;
	while (true) {
		if (coords.size() == 0 || count >= 1000) {
			this->onNoPath();
			return;
		}
		count++;
		// Get the coord with the minimum score
		sf::Vector2i c = coords.front();
		coords.erase(coords.begin());
		// Check if the coord is the target
		if (c == targetCoord) {
			std::vector<sf::Vector2f> trail;
			trail.push_back(sf::Vector2f(c) * 64.0f);
			while (true) {
				sf::Vector2i next = steps[c.x][c.y].first;
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
			std::vector<sf::Vector2i> toAdd;
			for (auto xOff : { -1, 0, 1 }) {
				for (auto yOff : { -1, 0, 1 }) {
					// Don't add the coord it's on
					if (xOff == 0 && yOff == 0) continue;
					if (xOff != 0 && yOff != 0) continue;
					// Check the tile d is sea
					if (gMap->getTileAt(c.x + xOff, c.y + yOff) == GameMap::TileType::Sea) {
						// Add the tile
						toAdd.push_back(sf::Vector2i(c.x + xOff, c.y + yOff));
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
			// Sort the coords by distance to target
			std::sort(coords.begin(), coords.end(), [&](sf::Vector2i a, sf::Vector2i b) {
				return pow(a.x - targetCoord.x, 2) + pow(a.y - targetCoord.y, 2)
					< pow(b.x - targetCoord.x, 2) + pow(b.y - targetCoord.y, 2);
			});
		}
	}
}
// Empty, may be filled in by subclass
void AutomatedShipController::onReachingTarget() {

}
void AutomatedShipController::onNoPath() {}
sf::Vector2f AutomatedShipController::getCoordsForEntity(std::weak_ptr<Entity> e) {
	if (auto b = std::dynamic_pointer_cast<BaseController>(e.lock()->components.controller)) {
		return b->getDockCoords();
	}
	else {
		return e.lock()->components.transform->getPosition();
	}
}