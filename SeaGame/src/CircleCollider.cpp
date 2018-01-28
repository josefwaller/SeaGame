#include "CircleCollider.h"

std::vector<sf::Vector2f> CircleCollider::BASE_POINTS = {};

CircleCollider::CircleCollider(std::weak_ptr<Entity> e, float r): ColliderComponent(e)
{
	if (CircleCollider::BASE_POINTS.size() == 0) {
		CircleCollider::BASE_POINTS = std::vector<sf::Vector2f>(CircleCollider::NUM_OF_POINTS);
		for (size_t i = 0; i < CircleCollider::NUM_OF_POINTS; i++) {
			CircleCollider::BASE_POINTS[i] = {
				(float)cos(2 * M_PI * i / CircleCollider::NUM_OF_POINTS),
				(float)sin(2 * M_PI * i / CircleCollider::NUM_OF_POINTS)
			};
		}
	}
	sf::Vector2f pOne = CircleCollider::BASE_POINTS[0];
	sf::Vector2f pTwo = CircleCollider::BASE_POINTS[1];
	for (auto it = CircleCollider::BASE_POINTS.begin() + 2; it != CircleCollider::BASE_POINTS.end(); it++) {
		this->addTriangle(Triangle({
			pOne * r,
			pTwo * r,
			*it * r
		}));
		pTwo = *it;
	}
}