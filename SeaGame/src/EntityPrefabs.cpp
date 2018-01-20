#include "EntityPrefabs.h"
#include "ShipRenderer.h"
#include "PlayerShipController.h"

std::shared_ptr<Entity> EntityPrefabs::playerShip(Game* g)
{
	auto ship = std::shared_ptr<Entity>(new Entity(g));
	ship->renderer = std::unique_ptr<RenderComponent>(new ShipRenderer(ship, "red"));
	ship->transform = std::shared_ptr<TransformComponent>(new TransformComponent(ship, { 30, 30 }, M_PI / 6));
	ship->controller = std::shared_ptr<ControllerComponent>(new PlayerShipController(ship));
	return ship;
}