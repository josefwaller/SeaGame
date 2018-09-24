#include "Box2dClick.h"
#include "Box2dTransform.h"

Box2dClick::Box2dClick(std::weak_ptr<Entity> parent) : ClickComponent(parent) {
}

bool Box2dClick::checkForClick(sf::Vector2f mousePos) {
	if (auto trans = std::dynamic_pointer_cast<Box2dTransform>(this->getParent().lock()->transform)) {
		auto body = trans->getBody();
		for (auto fix = body->GetFixtureList(); fix; fix = fix->GetNext()) {
			if (fix->TestPoint(b2Vec2(mousePos.x, mousePos.y))) {
				return true;
			}
		}
	}
	return false;
}