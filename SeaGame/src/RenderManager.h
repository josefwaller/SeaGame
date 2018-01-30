#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

class RenderManager
{
public:
	// Draw all sprites on the window
	void render(sf::RenderWindow& window);
	// Add a drawable to be drawn
	void addPointer(sf::Drawable* s, size_t index);
	// Convienence method so that new does not have to be called every time a sprite is added
	template<class T>
	void add(T s, const size_t index)
	{
		static_assert(std::is_base_of<sf::Drawable, T>::value, "U R dum");
		this->addPointer(new T(s), index);
	};
	// Remove all sprites
	void reset();
	// Z indexes to draw things on
	// For sea tiles, land tiles, etc
	const static size_t INDEX_TILES = 0;
	// For ship hulls
	const static size_t INDEX_HULL = 1;
	// For cannonballs
	const static size_t INDEX_CANNONBALLS = 2;
	// For things on the decks of ships
	const static size_t INDEX_DECK = 3;
	// For the ship sails
	const static size_t INDEX_SAILS = 4;
	// For explosions, fire, etc
	const static size_t INDEX_EFFECT = 5;
	// For debug information, hitboxes, etc
	const static size_t INDEX_DEBUG = 6;
private:
	std::vector<std::vector<std::shared_ptr<sf::Drawable>>> sprites;
};