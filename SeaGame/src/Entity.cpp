#include "Entity.h"
#include <algorithm>

unsigned int Entity::trueEntityCount = 0;

std::vector<ComponentType> allTypes = {
	ComponentType::Transform,
	ComponentType::Renderer,
	ComponentType::Physics,
	ComponentType::Controller,
	ComponentType::Health,
	ComponentType::Inventory,
	ComponentType::Gui,
	ComponentType::Click
};

std::map<std::string, std::string> Entity::getSaveData() {
	std::map<std::string, std::string> data = {
		{"type", std::to_string((int)this->type)}
	};
	// Iterate over a pointer of each component this->entity has
	for (ComponentType t : allTypes) {
		if (auto c = this->components.get(t)) {
			auto m = c->getSaveData();
			data.insert(m.begin(), m.end());
		}
	}
	return data;
}