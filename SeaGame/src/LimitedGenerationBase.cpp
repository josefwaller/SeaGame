#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "LimitedGenBaseController.h"
#include "MiningBaseRenderer.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::limitedGenerationBase(Game* g, sf::Vector2f pos, GameResource res) {
	return EntityPrefabs::buildEntity(new Entity(
		g,
		1,
		EntityType::MiningBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(EntityPrefabs::getBaseBody(g, pos, 0.0f)),
			new LimitedGenBaseController(res, 200),
			new MiningBaseRenderer(res),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
