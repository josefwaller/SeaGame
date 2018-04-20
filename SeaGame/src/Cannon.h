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
					parent.lock()->transform->getPosition() + position,
					rotation
				)
			);
			// Reset the clock
			fireClock.restart();
		}
	}
};