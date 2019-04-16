#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "PirateBaseController.h"
#include "PirateBaseRenderer.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::pirateBase(Game* g, sf::Vector2f pos) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		1,
		EntityType::PirateBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getBaseBody(g, pos, 0.0f)),
			new PirateBaseController(),
			new PirateBaseRenderer(),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
