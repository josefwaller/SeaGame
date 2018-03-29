#include "RenderComponent.h"
#include "Box2dTransform.h"

RenderComponent::RenderComponent(std::weak_ptr<Entity> newParent): Component(newParent)
{
}

void RenderComponent::renderCollider(RenderManager& r)
{
	if (std::shared_ptr<Box2dTransform> trans = std::dynamic_pointer_cast<Box2dTransform>(this->getParent()->transform)) {
		// Get the body
		b2Body* body = trans->getBody();
		b2Transform bodyTrans = body->GetTransform();
		// Go through each fixture
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			// Get shape, if it is  a rectangleshape
			b2PolygonShape* shape = (b2PolygonShape*)fixture->GetShape();
			// Create new vertex array to draw
			sf::VertexArray drawableVerts(sf::LineStrip, shape->m_count);
			for (int i = 0; i < shape->m_count; i++) {
				// Convert to world coordinates vs relative to the body
				b2Vec2 vert = body->GetWorldPoint(shape->m_vertices[i]);
				// Add a vertex to the SFML array to draw
				drawableVerts[i] = sf::Vertex({ vert.x, vert.y }, sf::Color::Red);
			}
			// Add the first vertice again, to make it a full loop
			drawableVerts.append(drawableVerts[0]);
			// Draw the vertex array
			r.add(drawableVerts, RenderManager::INDEX_DEBUG);
		}
	}
}
void RenderComponent::reset()
{

}