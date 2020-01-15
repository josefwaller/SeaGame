#include "EntityPrefabs.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "Box2dTransform.h"
#include "PirateFortressController.h"
#include "SpriteRenderer.h"
#include "HealthComponent.h"
#include "InventoryComponent.h"
#include "GuiComponent.h"
#include "Box2dClick.h"

entity_ptr EntityPrefabs::pirateFortress(Game* g, sf::Vector2f pos) {
	sf::Sprite spr = ResourceManager::get()->getSprite(
		"medievalRTS_spritesheet@2",
		"medievalStructure_05.png",
		false);
	spr.setScale(sf::Vector2f(1, 1) / spr.getLocalBounds().width * 64.0f * 3.0f);
	return buildEntity(new Entity(
		g,
		2,
		EntityType::PirateFortress,
		EntityTag::Base,
		ComponentList(
			new Box2dTransform(getBaseBody(g, pos, 0.0f)),
			new PirateFortressController(),
			new SpriteRenderer(spr, RenderManager::INDEX_BASE_TILES),
			nullptr,
			new HealthComponent(200),
			new InventoryComponent(),
			new GuiComponent(),
			new Box2dClick()
		)
	));
}
