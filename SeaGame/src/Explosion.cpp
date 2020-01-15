#include "EntityPrefabs.h"
#include "Entity.h"
#include "BasicTransform.h"
#include "AnimationRenderer.h"
#include "ResourceManager.h"

entity_ptr EntityPrefabs::explosion(Game* g, sf::Vector2f pos)
{
	return EntityPrefabs::buildEntity(new Entity(
		g,
		0,
		EntityType::Explosion,
		EntityTag::Effect,
		ComponentList(
			new BasicTransform(pos, 0.0f),
			nullptr,
			new AnimationRenderer(
				{
					ResourceManager::get()->getSprite("ships", "explosion3.png", true),
					ResourceManager::get()->getSprite("ships", "explosion2.png", true),
					ResourceManager::get()->getSprite("ships", "explosion1.png", true)
				},
				100.0f,
				RenderManager::INDEX_EFFECT,
				false
			),
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		)
	));
}
