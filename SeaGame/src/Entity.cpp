#include "Entity.h"
#include "Game.h"
#include <algorithm>

unsigned int Entity::trueEntityCount = 0;

unsigned long Entity::currentId = 0;
std::map<std::string, std::string> Entity::getSaveData() {
	std::map<std::string, std::string> data = {
		{"type", std::to_string((int)this->type)},
		{"id", std::to_string(this->id)}
	};
	// Iterate over a pointer of each component this->entity has
	for (ComponentType t : ComponentList::allTypes) {
		if (auto c = this->components.get(t)) {
			auto m = c->getSaveData();
			data.insert(m.begin(), m.end());
		}
	}
	return data;
}