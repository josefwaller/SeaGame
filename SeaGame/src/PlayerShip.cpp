#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "PlayerShipController.h"
#include "ShipRenderer.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::playerShip(Game* g, sf::Vector2f position)
{
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::Player,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getShipBody(g, position, 0.0f)),
			new PlayerShipController(),
			new ShipRenderer(ShipRenderer::SailColor::Blue),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
