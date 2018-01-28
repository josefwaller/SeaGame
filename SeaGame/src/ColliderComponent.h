#pragma once
#include "Component.h"
#include "Triangle.h"

class ColliderComponent : public Component
{
public:
	ColliderComponent(std::weak_ptr<Entity> parent);
	// Check for collision
	virtual bool checkForCollision(std::weak_ptr<ColliderComponent> other);
	// Get the triangles this collider uses
	std::vector<Triangle> getTriangles();
	void addTriangle(Triangle t);
	sf::FloatRect getBoundingBox();
private:
	std::vector<Triangle> triangles;
	// The point at with the minimum x value and the minimum y value of all the triangles
	sf::Vector2f minPoint;
	// Same as minPoint, but maximum values instead
	sf::Vector2f maxPoint;
};