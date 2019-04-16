#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "MilitaryBaseController.h"
#include "MilitaryBaseRenderer.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::militaryBase(Game* g, sf::Vector2f pos)
{
	return buildEntity(new Entity(
		g,
		0,
		EntityType::MilitaryBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getBaseBody(g, pos, 0.0f)),
			new MilitaryBaseController(),
			new MilitaryBaseRenderer(),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
