#include "LayoutSprite.h"

void positionLayoutSprite(sf::Sprite& spr, LayoutSprite lSpr, sf::Vector2f pos, float rot)
{
	sf::Transform trans;
	trans.rotate(rot);

	spr.setPosition(pos + trans.transformPoint(lSpr.pos));
	spr.setRotation(rot + lSpr.rot);
}