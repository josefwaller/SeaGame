#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "AttackPirateShipController.h"
#include "ShipRenderer.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::attackPirateShip(Game* g, sf::Vector2f pos, std::weak_ptr<Entity> base, std::weak_ptr<Entity> target) {
	return buildEntity(new Entity(
		g,
		1,
		EntityType::AttackPirateShip,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(getShipBody(g, pos, 0.0f)),
			new AttackPirateShipController(base, target),
			new ShipRenderer(ShipRenderer::SailColor::Black),
			new PhysicsComponent,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
