#include "Cannon.h"
#include "EntityPrefabs.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "Game.h"

Cannon::Cannon() {}
Cannon::Cannon(std::weak_ptr<Entity> p, sf::Vector2f pos, float rot, float fireInt) {
	parent = p;
	position = pos;
	rotation = rot;
	fireInterval = fireInt;
}
void Cannon::fire() {
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
void Cannon::autoAim(std::weak_ptr<Entity> target) {
	if (target.lock()) {
		sf::Vector2f targetPos = target.lock()->components.transform->getPosition();
		sf::Vector2f diff = targetPos - (this->parent.lock()->components.transform->getPosition() + this->position);
		this->rotation = atan2f(diff.y, diff.x);
	}
}
// Find a suitable target within the range given
std::weak_ptr<Entity> Cannon::findTarget(float range) {
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
