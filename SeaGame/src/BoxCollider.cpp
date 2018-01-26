#include "BoxCollider.h"

BoxCollider::BoxCollider(std::weak_ptr<Entity> parent, sf::Vector2f pos, int w, int h) : ColliderComponent(parent)
{
	this->triangles = {
		Triangle({
			{pos.x, pos.y},
			{pos.x + w, pos.y},
			{pos.x + w, pos.y + h}
		}),
		Triangle({
			{pos.x, pos.y},
			{pos.x, pos.y + h},
			{pos.x + w, pos.y + h}
		})
	};
}