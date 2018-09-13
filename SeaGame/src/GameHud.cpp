#include "GameHud.h"
#include "EntityPrefabs.h"
#include "CraftRecipes.h"
#include "TechTree.h"

GameHud::GameHud() {
}
GameHud::GameHud(Game* g) {
	this->game = g;
	// Add the build button
	this->buildButton = tgui::Button::create();
	this->buildButton->setText("Build");
	this->game->getGui().add(this->buildButton);
	this->buildButton->connect("clicked", &GameHud::toggleBuildButtons, game->getHud());
	this->currentClickState = ClickState::Nothing;
	this->researchButton = tgui::Button::create();
	this->researchButton->setText("Research");
	this->researchButton->setPosition({ 500.0f, 0 });
	this->researchButton->connect("clicked", [&](Game* g) {
		g->getHud()->toggleResearchButtons();
	}, this->game);
	this->game->getGui().add(this->researchButton);
}
void GameHud::tryToBuild(CraftingRecipes::CraftRecipe cr, sf::Vector2f pos) {
	// Get the player's inventory
	auto player = this->game->getPlayer();
	auto inv = player->inventory->getInventory();
	// Check the player has all the resources required
	for (auto res : cr.requiredResources) {
		if (inv[res.first] < res.second) {
			return;
		}
	}
	// Add the thingy
	if (cr.createMethod(this->game, pos)) {
		// Remove the required resources
		for (auto res : cr.requiredResources) {
			player->inventory->removeItems(res.first, res.second);
		}
	}
}
void GameHud::transferItems(std::weak_ptr<Entity> e, GameResource res, unsigned int amount) {
	this->selectCallback = std::bind([&](std::weak_ptr<Entity> to, std::weak_ptr<Entity> from) {
		// TODO, make sure entities are close to each other
		if (to.lock() && from.lock()) {
			std::map<GameResource, unsigned int> resources = from.lock()->inventory->getInventory();
			// Remove resources from entity one
			for (auto it : resources) {
				from.lock()->inventory->removeItems(it.first, it.second);
			}
			// Add resources to entity two
			for (auto it : resources) {
				to.lock()->inventory->addItems(it.first, it.second);
			}
		}
	}, std::placeholders::_1, e);
	this->currentClickState = ClickState::Selecting;
}
void GameHud::onClick(sf::Vector2f pos) {
	if (this->currentClickState == ClickState::Nothing) {
		for (auto e : this->game->getEntities()) {
			if (e->click != nullptr) {
				if (e->click->checkForClick(pos)) {
					if (e->controller != nullptr) {
						e->controller->onClick();
					}
				}
			}
		}
	}
	else if (this->currentClickState == ClickState::Building) {
		this->clickCallback(this->game, pos);
		this->currentClickState = ClickState::Nothing;
	}
	else if (this->currentClickState == ClickState::Selecting) {
		for (auto e : this->game->getEntities()) {
			if (e->click) {
				if (e->click->checkForClick(pos)) {
					this->selectCallback(e);
					this->currentClickState = ClickState::Nothing;
				}
			}
		}
	}
}
void GameHud::selectPoint(ClickState c, std::function<void(Game* g, sf::Vector2f pos)> func) {
	this->currentClickState = c;
	this->clickCallback = func;
}
void GameHud::selectEntity(std::function<void(std::weak_ptr<Entity> entity)> callback) {
	this->selectCallback = callback;
	this->currentClickState = ClickState::Selecting;
}
void GameHud::toggleBuildButtons() {
	this->resetBuildButtons();
	for (auto btn : this->toBuildButtons) {
		this->game->getGui().add(btn);
	}
}
void GameHud::resetResearchButtons() {
	std::vector<tgui::Button::Ptr> buttons;
	float height = this->researchButton->getFullSize().y;
	for (auto it : TechTree::nodes) {
		if (it.second.parent == Technology::Nothing
			|| TechTree::nodes.find(it.second.parent)->second.isResearched) {
			buttons.push_back(tgui::Button::create());
			auto btn = buttons.back();
			if (it.second.isResearched) {
				btn->setEnabled(false);
			}
			btn->setText(it.second.name);
			btn->setPosition({ 500.0f, height });
			btn->connect("clicked", [&](Game* g, Technology tech) {
				// Check the player has the correct resources for the technology
				TechTreeNode* node = &TechTree::nodes.find(tech)->second;
				std::shared_ptr<Entity> player = g->getPlayer();
				std::map<GameResource, unsigned int> playerInventory = player->inventory->getInventory();
				// Check for each item in the player's inventory
				for (auto invIt : node->requiredResources) {
					if (playerInventory[invIt.first] < invIt.second) {
						return;
					}
				}
				// Remove the items
				for (auto invIt : node->requiredResources) {
					player->inventory->removeItems(invIt.first, invIt.second);
				}
				// Set the node to researched
				node->isResearched = true;
				// Reset the buttons
				g->getHud()->toggleResearchButtons();
				g->getHud()->toggleBuildButtons();
			}, this->game, it.first);
			height += btn->getFullSize().y;
		}
	}
	this->toResearchButtons = buttons;
}
void GameHud::resetBuildButtons() {
	// Add the build buttons
	float x = 0.0f;
	float y = this->buildButton->getFullSize().y;
	for (auto cr : CraftingRecipes::recipes) {
		// Make sure the user can build this thing
		if (cr.requiredTech == Technology::Nothing || TechTree::nodes[cr.requiredTech].isResearched) {
			// Add the actual button
			this->toBuildButtons.push_back(tgui::Button::create());
			tgui::Button::Ptr btn = this->toBuildButtons.back();
			btn->setText(cr.displayText);
			btn->connect("clicked", [&](Game* g, CraftingRecipes::CraftRecipe cr) {
				g->getHud()->selectPoint(ClickState::Building, std::bind(
					[&](Game* g, sf::Vector2f pos, CraftingRecipes::CraftRecipe cr) {
					g->getHud()->tryToBuild(cr, pos);
				}, std::placeholders::_1, std::placeholders::_2, cr));
			}, this->game, cr);
			btn->setPosition({ x, y });
			x += btn->getFullSize().x;
		}
	}
}
void GameHud::toggleResearchButtons() {
	this->resetResearchButtons();
	for (auto b : this->toResearchButtons) {
		this->game->getGui().add(b);
	}
}
