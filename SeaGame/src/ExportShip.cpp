#include "EntityPrefabs.h"
#include "Entity.h"
#include "Game.h"
#include "Box2dTransform.h"
#include "ExportShipController.h"
#include "ShipRenderer.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::exportShip(Game* g, sf::Vector2f pos, std::weak_ptr<Entity> base) {
	return buildEntity(new Entity(
		g,
		0,
		EntityType::ExportShip,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(getShipBody(g, pos, 0.0f)),
			new ExportShipController(base),
			new ShipRenderer(ShipRenderer::SailColor::Yellow),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}