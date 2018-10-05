#pragma once
#include "EntityPrefabs.h"
// Represent a cannon in the game. Has position and direction, and also can aim at points.
// Also has a fire interval which tracks how long it must wait before firing again
struct Cannon
{
	Cannon() {}
	Cannon(std::weak_ptr<Entity> p, sf::Vector2f pos, float rot, float fireInt) {
		parent = p;
		position = pos;
		rotation = rot;
		fireInterval = fireInt;
	}
	// Entity which has the cannon
	std::weak_ptr<Entity> parent;
	// Position relative to the parent
	// Does not account for parent's rotation
	sf::Vector2f position;
	// Rotation
	float rotation;
	// Keep track of how long to wait before firing
	sf::Clock fireClock;
	float fireInterval;
	// Fires the cannon
	void fire() {
		if (fireClock.getElapsedTime().asSeconds() > fireInterval) {
			// Create a new cannonball
			parent.lock()->game->addEntity(
				EntityPrefabs::cannonBall(
					parent.lock()->game,
					parent,
					parent.lock()->components.transform->getPosition() + position,
					rotation
				)
			);
			// Reset the clock
			fireClock.restart();
		}
	}
	// Automatically aims the cannon at a target within its range
	void autoAim(std::weak_ptr<Entity> target) {
		if (target.lock()) {
			sf::Vector2f targetPos = target.lock()->components.transform->getPosition();
			sf::Vector2f diff = targetPos - (this->parent.lock()->components.transform->getPosition() + this->position);
			this->rotation = atan2f(diff.y, diff.x);
		}
	}
	// Find a suitable target within the range given
	std::weak_ptr<Entity> findTarget(float range) {
		for (auto it : this->parent.lock()->game->getEntities()) {
			sf::Vector2f diff = it->components.transform->getDifference(
				this->parent.lock()->components.transform->getPosition() + this->position
			).first;
			if (pow(diff.x, 2) + pow(diff.y, 2) <= pow(range, 2)) {
				return it;
			}
		}
		return {};
	}
};