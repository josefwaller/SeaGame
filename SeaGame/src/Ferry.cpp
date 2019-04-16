#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "FerryShipController.h"
#include "ShipRenderer.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"


entity_ptr EntityPrefabs::ferryShip(Game* g, sf::Vector2f pos, std::weak_ptr<Entity> from, std::weak_ptr<Entity> to) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::Ferry,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getShipBody(g, pos, 0.0f)),
			new FerryShipController(),
			new ShipRenderer(ShipRenderer::SailColor::Red),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
