#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "ChasingShipController.h"
#include "ShipRenderer.h"
#include "PhysicsComponent.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::enemyChasingShip(Game* g, sf::Vector2f pos)
{
	return buildEntity(new Entity(
		g,
		1,
		EntityType::PirateShip,
		EntityTag::Ship,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getShipBody(g, pos, 0.0f)),
			new ChasingShipController(),
			new ShipRenderer(ShipRenderer::SailColor::Black),
			new PhysicsComponent(),
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
