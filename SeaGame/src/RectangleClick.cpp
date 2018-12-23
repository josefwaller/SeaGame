#include "RectangleClick.h"

RectangleClick::RectangleClick(sf::Vector2i offset, sf::Vector2i size)
	: rect(sf::Vector2f(offset), sf::Vector2f(size)) {
}
bool RectangleClick::checkForClick(sf::Vector2f pos) {
	// Get copy of rect
	sf::FloatRect r = this->rect;
	// Transform rectangle to entity's position and rotation
	if (auto trans = this->getParent().lock()->components.transform) {
		sf::Transform t;
		t.rotate(trans->getRotation());
		t.translate(trans->getPosition());
		r = t.transformRect(r);
	}
	// Check if mouse point is contained
	return r.contains(pos);
}