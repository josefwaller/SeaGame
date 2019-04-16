#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "DefensePirateShipController.h"
#include "ShipRenderer.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::defensePirateShip(Game* g, sf::Vector2f pos, std::weak_ptr<Entity> base) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		1,
		EntityType::DefensePirateShip,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(getShipBody(g, pos, 0.0f)),
			new DefensePirateShipController(base),
			new ShipRenderer(ShipRenderer::SailColor::Black),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
