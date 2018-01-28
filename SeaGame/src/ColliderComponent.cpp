#include "ColliderComponent.h"
#include <iostream>

ColliderComponent::ColliderComponent(std::weak_ptr<Entity> parent) : Component(parent)
{

}
bool ColliderComponent::checkForCollision(std::weak_ptr<ColliderComponent> other)
{
	if (this->getBoundingBox().intersects(other.lock()->getBoundingBox())) {
		for (auto t : this->getTriangles()) {
			for (auto o : other.lock()->getTriangles()) {
				if (t.collidesWith(o)) {
					return true;
				}
			}
		}
	}
	return false;
}
std::vector<Triangle> ColliderComponent::getTriangles()
{
	// Create copy of entity's triangles and move then to the entity's position/rotation
	sf::Transform trans;
	trans.translate(this->getParent()->transform->getPosition());
	trans.rotate(this->getParent()->transform->getRotation() * 180.0f / M_PI);
	std::vector<Triangle> toReturn;
	for (auto t : this->triangles) {
		toReturn.push_back(Triangle({
			trans.transformPoint(t.points[0]),
			trans.transformPoint(t.points[1]),
			trans.transformPoint(t.points[2])
		}));
	}
	return toReturn;
}
void ColliderComponent::addTriangle(Triangle t)
{
	for (auto it : t.points) {
		this->minPoint = {
			std::min(it.x, minPoint.x),
			std::min(it.y, minPoint.y)
		};
		this->maxPoint = {
			std::max(it.x, maxPoint.x),
			std::max(it.y, maxPoint.y)
		};
	}
	this->triangles.push_back(t);
}
sf::FloatRect ColliderComponent::getBoundingBox()
{
	// Get the four points of the bounding box rectangle
	std::vector<sf::Vector2f> points = {
		this->minPoint,
		sf::Vector2f(this->maxPoint.x, this->minPoint.y),
		sf::Vector2f(this->minPoint.x, this->maxPoint.y),
		this->maxPoint
	};
	// Transform all of them to get the current coordinate
	std::shared_ptr<TransformComponent> trans = this->getParent()->transform;
	sf::Transform t;
	t.translate(trans->getPosition());
	t.rotate(trans->getRotation() * 180.0f / M_PI);
	for (sf::Vector2f& p : points) {
		p = t.transformPoint(p);
	}
	// Get a point with the min x and y value, and the max x and y value
	sf::Vector2f minP = points[0];
	sf::Vector2f maxP = points[0];
	for (sf::Vector2f p : points) {
		// Set minimum point
		minP.x = std::min(minP.x, p.x);
		minP.y = std::min(minP.y, p.y);
		// Set maximum point
		maxP.x = std::max(maxP.x, p.x);
		maxP.y = std::max(maxP.y, p.y);
	}
	// Return rectangle
	return sf::FloatRect(
		minP.x,
		minP.y,
		maxP.x - minP.x,
		maxP.y - minP.y
	);
}