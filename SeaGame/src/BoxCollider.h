#pragma once
#include "ColliderComponent.h"

class BoxCollider : public ColliderComponent
{
public:
	BoxCollider(std::weak_ptr<Entity> parent, sf::Vector2f pos, int w, int h);
};