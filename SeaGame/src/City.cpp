#include "EntityPrefabs.h"
#include "Entity.h"
#include "Box2dTransform.h"
#include "CityController.h"
#include "CityRenderer.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::city(Game* g, sf::Vector2f pos) {
	return buildEntity(new Entity(
		g,
		0,
		EntityType::City,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(getBaseBody(g, pos, 0.0f)),
			new CityController(),
			new CityRenderer(),
			nullptr,
			nullptr,
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
