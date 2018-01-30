#pragma once
#include "ColliderComponent.h"

class CircleCollider : public ColliderComponent
{
public:
	static const size_t NUM_OF_POINTS = 5;
	static std::vector<sf::Vector2f> BASE_POINTS;

	CircleCollider(std::weak_ptr<Entity> e, float r);
private:
};