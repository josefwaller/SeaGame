#include "ColliderComponent.h"

ColliderComponent::ColliderComponent(std::weak_ptr<Entity> parent) : Component(parent)
{

}
bool ColliderComponent::checkForCollision(std::weak_ptr<ColliderComponent> other)
{
	for (auto t : this->triangles) {
		for (auto o : other.lock()->getTriangles()) {
			if (t.collidesWith(o)) {
				return true;
			}
		}
	}
	return false;
}
std::vector<Triangle> ColliderComponent::getTriangles()
{
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