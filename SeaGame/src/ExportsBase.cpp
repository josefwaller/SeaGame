#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "ExportBaseController.h"
#include "BaseRenderer.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"
#include "Game.h"

entity_ptr EntityPrefabs::exportBase(Game* g, sf::Vector2f pos) {
	return buildEntity(new Entity(
		g,
		0,
		EntityType::ExportBase,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(getBaseBody(g, pos, 0.0f)),
			new ExportBaseController(),
			new BaseRenderer(),
			nullptr,
			new HealthComponent(100),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}