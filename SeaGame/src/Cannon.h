#pragma once
// Represent a cannon in the game. Has position and direction, and also can aim at points.
// Also has a fire interval which tracks how long it must wait before firing again
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
struct Entity;
struct Cannon
{
	Cannon();
	Cannon(std::weak_ptr<Entity> p, sf::Vector2f pos, float rot, float fireInt);
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
	void fire();
	// Automatically aims the cannon at a target within its range
	void autoAim(std::weak_ptr<Entity> target);
	// Find a suitable target within the range given
	std::weak_ptr<Entity> findTarget(float range);
};