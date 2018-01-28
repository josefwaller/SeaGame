#include "RenderComponent.h"

RenderComponent::RenderComponent(std::weak_ptr<Entity> newParent): Component(newParent)
{
}

void RenderComponent::renderCollider(RenderManager& r)
{
	// Check parent has collider
	if (this->getParent()->collider != nullptr) {
		for (auto t : this->getParent()->collider->getTriangles()) {
			sf::VertexArray p(sf::LineStrip, 4);
			for (size_t i = 0; i < 4; i++) {
				p[i] = t.points[i % 3];
				p[i].color = sf::Color::Red;
			}
			r.add(p, RenderManager::INDEX_DEBUG);
		}
	}
}