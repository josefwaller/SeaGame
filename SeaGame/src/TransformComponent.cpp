#include "TransformComponent.h"
#include "MilitaryBaseController.h"

TransformComponent::TransformComponent(std::weak_ptr<Entity> parent): Component(parent)
{

}

std::pair<sf::Vector2f, float> TransformComponent::getDifference(std::weak_ptr<TransformComponent> other)
{
	return getDifference(other.lock()->getPosition());
}
std::pair<sf::Vector2f, float> TransformComponent::getDifference(sf::Vector2f other)
{
	sf::Vector2f diff = other - this->getPosition();
	// For some reason, the range of atan in c++ is [-pi, pi] instead of [0, 2pi]
	// So the next 3 lines convert it to [0, 2pi]
	float angle = atan2(diff.y, diff.x);
	if (angle < 0)
		angle += (float)(2 * M_PI);
	// Get difference between this entity's rotation and the angle to the other
	angle -= this->getRotation();
	// If the angle is greater than pi, it is shorter to go the negative direction
	if (angle > M_PI)
		angle -= (float)(2 * M_PI);
	else if (angle < -M_PI)
		angle += (float)(2 * M_PI);

	return { diff, angle };

}