#include "RenderComponent.h"
#include "Box2dTransform.h"

void RenderComponent::renderCollider(RenderManager& r)
{
	if (std::shared_ptr<Box2dTransform> trans = std::dynamic_pointer_cast<Box2dTransform>(this->getParent().lock()->components.transform)) {
		// Get the body
		b2Body* body = trans->getBody();
		b2Transform bodyTrans = body->GetTransform();
		// Go through each fixture
		for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			// Get shape, if it is  a rectangleshape
			if (b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(fixture->GetShape())) {
				// Create new vertex array to draw
				sf::VertexArray drawableVerts(sf::LineStrip, shape->m_count);
				for (int i = 0; i < shape->m_count; i++) {
					// Convert to world coordinates vs relative to the body
					b2Vec2 vert = body->GetWorldPoint(shape->m_vertices[i]);
					// Add a vertex to the SFML array to draw
					drawableVerts[i] = sf::Vertex({ vert.x * Game::BOX2D_TO_WORLD, vert.y * Game::BOX2D_TO_WORLD }, sf::Color::Red);
				}
				// Add the first vertice again, to make it a full loop
				drawableVerts.append(drawableVerts[0]);
				// Draw the vertex array
				r.add(drawableVerts, RenderManager::INDEX_DEBUG);
			}
			else if (b2CircleShape* shape = dynamic_cast<b2CircleShape*>(fixture->GetShape())) {
				// Draw the circle collider is it has one
				sf::CircleShape drawableShape;
				drawableShape.setRadius(shape->m_radius * Game::BOX2D_TO_WORLD);
				b2Vec2 worldPos = body->GetWorldPoint(shape->m_p);
				drawableShape.setPosition({ worldPos.x * Game::BOX2D_TO_WORLD, worldPos.y * Game::BOX2D_TO_WORLD });
				drawableShape.setOutlineColor(sf::Color::Red);
				drawableShape.setOutlineThickness(2);
				drawableShape.setFillColor(sf::Color::Transparent);
				r.add(drawableShape, RenderManager::INDEX_DEBUG);
			}
		}
	}
}
void RenderComponent::reset()
{

}