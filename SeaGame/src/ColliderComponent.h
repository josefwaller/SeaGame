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
protected:
	std::vector<Triangle> triangles;
};