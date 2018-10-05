#include "Entity.h"
#include <algorithm>

unsigned int Entity::trueEntityCount = 0;

std::map<std::string, std::string> Entity::getSaveData() {
	std::map<std::string, std::string> data = {
		{"type", std::to_string((int)this->type)}
	};
	// Iterate over a pointer of each component this entity has
	for (Component* it : {
		(Component*)this->transform.get(),
		(Component*)this->renderer.get(),
		(Component*)this->controller.get(),
		(Component*)this->physics.get(),
		(Component*)this->health.get(),
		(Component*)this->inventory.get(),
		(Component*)this->gui.get()
	}) {
		if (it) {
			auto m = it->getSaveData();
			data.insert(m.begin(), m.end());
		}
	}
	return data;
}