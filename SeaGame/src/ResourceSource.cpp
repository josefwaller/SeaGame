#include "EntityPrefabs.h"
#include "Entity.h"
#include "BasicTransform.h"
#include "ResourceController.h"
#include "ResourceRenderer.h"
#include "InventoryComponent.h"
#include "RectangleClick.h"

entity_ptr EntityPrefabs::resourceSource(Game* g, sf::Vector2f pos, GameResource res) {
	return buildEntity(new Entity(
		g,
		1,
		EntityType::ResourceDeposit,
		EntityTag::Resource,
		ComponentList(
			new BasicTransform(pos, 0.0f),
			new ResourceController(res, 200),
			new ResourceRenderer(res),
			nullptr,
			nullptr,
			new InventoryComponent(),
			nullptr,
			new RectangleClick(sf::Vector2i(0, 0), sf::Vector2i(64, 64))
		)
	));
}