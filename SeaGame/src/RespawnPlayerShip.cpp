#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "RespawnPlayerShipController.h"
#include "ShipRenderer.h"
#include "PhysicsComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::respawnPlayerShip(Game* g, sf::Vector2f pos) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::Player,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(getShipBody(g, pos, 0.0f)),
			new RespawnPlayerShipController(),
			new ShipRenderer(ShipRenderer::SailColor::White),
			new PhysicsComponent(),
			nullptr,
			nullptr,
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
