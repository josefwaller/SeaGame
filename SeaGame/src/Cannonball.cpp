#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "CannonballController.h"
#include "SpriteRenderer.h"
#include "PhysicsComponent.h"

entity_ptr EntityPrefabs::cannonBall(Game* g, int team, sf::Vector2f pos, float rot)
{
	// If loading, team will be set after the cannonball is created
	return buildEntity(new Entity(
		g,
		team,
		EntityType::Cannonball,
		EntityTag::Cannonball,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getCannonballBody(g, pos, rot)),
			new CannonballController(rot),
			new SpriteRenderer("ships", "cannonBall.png", RenderManager::INDEX_CANNONBALLS),
			new PhysicsComponent(),
			nullptr,
			nullptr,
			nullptr,
			nullptr
		)
	));
}
