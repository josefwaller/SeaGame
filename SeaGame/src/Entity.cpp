#include "Entity.h"
#include <algorithm>

unsigned int Entity::trueEntityCount = 0;

std::map<std::string, std::string> Entity::getSaveData() {
	std::map<std::string, std::string> data = {
		{"type", std::to_string((int)this->type)}
	};
	// Iterate over a pointer of each component this->entity has
	for (Component* it : {
		(Component*)this->components.transform.get(),
		(Component*)this->components.renderer.get(),
		(Component*)this->components.controller.get(),
		(Component*)this->components.physics.get(),
		(Component*)this->components.health.get(),
		(Component*)this->components.inventory.get(),
		(Component*)this->components.gui.get()
	}) {
		if (it) {
			auto m = it->getSaveData();
			data.insert(m.begin(), m.end());
		}
	}
	return data;
}