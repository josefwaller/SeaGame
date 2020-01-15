#include "ExportBaseController.h"
#include "EntityPrefabs.h"

void ExportBaseController::update(float delta) {
	if (!this->ship.lock()) {
		auto s = EntityPrefabs::exportShip(this->getGame(), this->getDockCoords(), this->getParent());
		this->ship = s;
		this->getGame()->addEntity(s);
	}
}