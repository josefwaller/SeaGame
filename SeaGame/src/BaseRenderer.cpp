#include "BaseRenderer.h"
#include "BaseController.h"
#include "ResourceManager.h"

BaseRenderer::BaseRenderer(std::weak_ptr<Entity> parent) : RenderComponent(parent)
{
	this->tower = ResourceManager::get()->getSprite("tiles", "tower", false);
	this->horizWall = ResourceManager::get()->getSprite("tiles", "wall-horizontal", false);
	this->vertWall = ResourceManager::get()->getSprite("tiles", "wall-vertical", false);
}
void BaseRenderer::render(RenderManager& r)
{
	auto trans = this->getParent().lock()->transform;
	// Draw walls connected the four corners
	renderHorizontalWall(r, trans->getPosition(), 3);
	renderHorizontalWall(r, trans->getPosition() + sf::Vector2f(0.0f, 2.0f * 64.0f), 3);
	renderVerticalWall(r, trans->getPosition(), 3);
	renderVerticalWall(r, trans->getPosition() + sf::Vector2f(2.0f * 64.0f, 0.0f), 3);

	// Draw the 4 towers at the corners
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			this->tower.setPosition(this->getParent().lock()->transform->getPosition() + sf::Vector2f(x * 2.0f * 64.0f, y * 2.0f * 64.0f));
			r.add(this->tower, RenderManager::INDEX_BASE_TILES);
		}
	}
	// Draw something at the dock coords
	if (auto cont = std::dynamic_pointer_cast<BaseController>(this->getParent().lock()->controller)) {
		sf::Vector2f dCoords = cont->getDockCoords();
		sf::Sprite s = ResourceManager::get()->getSprite("tiles", "tower", false);
		s.setPosition(dCoords);
		r.add(s, RenderManager::INDEX_BASE_TILES);
	}
}
void BaseRenderer::renderHorizontalWall(RenderManager& r, sf::Vector2f start, int length)
{
	// Draw the beginning of the wall
	sf::Sprite wallStart = ResourceManager::get()->getSprite("tiles", "wall-right", false);
	wallStart.setPosition(start);
	r.add(wallStart, RenderManager::INDEX_BASE_TILES);
	// Draw the main body of the wall
	sf::Sprite wallSprite = ResourceManager::get()->getSprite("tiles", "wall-horizontal", false);
	for (int x = 0; x < length - 2; x++) {
		wallSprite.setPosition(sf::Vector2f(start.x + (x + 1) * 64.0f, start.y));
		r.add(wallSprite, RenderManager::INDEX_BASE_TILES);
	}
	// End the wall
	sf::Sprite wallEnd = ResourceManager::get()->getSprite("tiles", "wall-left", false);
	wallEnd.setPosition(sf::Vector2f(start.x + (length - 1) * 64.0f, start.y));
	r.add(wallEnd, RenderManager::INDEX_BASE_TILES);
}
void BaseRenderer::renderVerticalWall(RenderManager& r, sf::Vector2f start, int length)
{
	// Draw the beginning of the wall
	sf::Sprite wallStart = ResourceManager::get()->getSprite("tiles", "wall-down", false);
	wallStart.setPosition(start);
	r.add(wallStart, RenderManager::INDEX_BASE_TILES);
	// Draw the main body of the wall
	sf::Sprite wallSprite = ResourceManager::get()->getSprite("tiles", "wall-vertical", false);
	for (int y = 0; y < length - 2; y++) {
		wallSprite.setPosition(sf::Vector2f(start.x, start.y + (y + 1) * 64.0f));
		r.add(wallSprite, RenderManager::INDEX_BASE_TILES);
	}
	// End the wall
	sf::Sprite wallEnd = ResourceManager::get()->getSprite("tiles", "wall-up", false);
	wallEnd.setPosition(sf::Vector2f(start.x, start.y + (length - 1) * 64.0f));
	r.add(wallEnd, RenderManager::INDEX_BASE_TILES);
}