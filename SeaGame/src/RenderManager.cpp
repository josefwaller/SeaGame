#include "RenderManager.h"

void RenderManager::reset() {
	this->sprites = {};
}

void RenderManager::addSprite(sf::Sprite s, size_t index)
{
	while (this->sprites.size() < index + 1) {
		this->sprites.push_back({});
	}
	this->sprites[index].push_back(s);
	auto x = 0;
}

void RenderManager::render(sf::RenderWindow& w)
{
	for (size_t o = 0; o < this->sprites.size(); o++) {
		for (size_t i = 0; i < this->sprites[o].size(); i++) {
			w.draw(this->sprites[o][i]);
		}
	}
}