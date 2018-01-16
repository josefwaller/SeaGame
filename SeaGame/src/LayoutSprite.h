#pragma once
#include <SFML/Graphics.hpp>

struct LayoutSprite
{
	sf::Vector2f pos;
	float rot;
};

void positionLayoutSprite(sf::Sprite& spr, LayoutSprite lSpr, sf::Vector2f pos, float rot);