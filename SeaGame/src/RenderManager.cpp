#include "RenderManager.h"

void RenderManager::reset() {
	this->sprites = {};
}
void RenderManager::addPointer(sf::Drawable* s, size_t index)
{
	while (this->sprites.size() < index + 1) {
		this->sprites.push_back({});
	}
	this->sprites[index].push_back(std::shared_ptr<sf::Drawable>(s));
}
void RenderManager::render(sf::RenderWindow* w)
{
	for (size_t o = 0; o < this->sprites.size(); o++) {
		for (size_t i = 0; i < this->sprites[o].size(); i++) {
			w->draw(*this->sprites[o][i]);
		}
	}
}