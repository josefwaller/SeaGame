#include "GameHud.h"
#include "MiningBaseController.h"
#include "EntityPrefabs.h"
#include "CraftRecipes.h"
#include "TechTree.h"
#include "InventoryComponent.h"
#include "ResourceController.h"

GameHud::GameHud() {
}
GameHud::GameHud(Game* g) {
	this->game = g;
	// Set click state to nothing initially
	this->currentClickState = ClickState::Nothing;
	// Add the build button
	this->buildButton = tgui::Button::create();
	this->buildButton->setText("Build");
	this->buildButton->connect("clicked", &GameHud::toggleBuildButtons, game->getHud());
	this->game->getGui()->add(this->buildButton);
	// Add the build container
	this->buildGroup = tgui::Group::create();
	this->buildGroup->setPosition({ 0, this->buildButton->getFullSize().y });
	this->buildGroup->setVisible(false);
	this->game->getGui()->add(this->buildGroup);
	// Add research button
	this->researchButton = tgui::Button::create();
	this->researchButton->setText("Research");
	this->researchButton->setPosition({ 500.0f, 0 });
	this->researchButton->connect("clicked", [&](Game* g) {
		g->getHud()->toggleResearchButtons();
	}, this->game);
	this->game->getGui()->add(this->researchButton);
	// Add research group
	this->researchGroup = tgui::Group::create();
	this->researchGroup->setVisible(false);
	this->game->getGui()->add(this->researchGroup);
	// Add save button
	this->saveBtn = tgui::Button::create();
	this->saveBtn->setText("Save");
	this->saveBtn->setPosition({ 250.0f, 0 });
	this->saveBtn->connect("clicked", &Game::save, this->game);
	this->game->getGui()->add(this->saveBtn);
	// Add money display
	this->moneyDisplay = tgui::Label::create();
	this->moneyDisplay->setPosition({ this->game->getWindow()->getSize().x - 300, 0 });
	this->game->getGui()->add(this->moneyDisplay);
	// Add player health display
	this->playerHealth = tgui::ProgressBar::create();
	this->playerHealth->getRenderer()->setFillColor(tgui::Color(255, 0, 0, 255));
	this->playerHealth->setMaximum(100);
	this->playerHealth->setPosition({ 0, this->game->getWindow()->getSize().y - this->playerHealth->getFullSize().y });
	this->game->getGui()->add(this->playerHealth);
}
void GameHud::update() {
	// Update money display
	this->moneyDisplay->setText("$" + std::to_string(this->game->getMoney()));
	// Update player health
	this->playerHealth->setValue(this->game->getPlayer()->components.health->getHealth());
}
void GameHud::tryToBuild(CraftingRecipes::CraftRecipe cr, sf::Vector2f pos) {
	// Get the player's inventory
	auto player = this->game->getPlayer();
	auto inv = player->components.inventory->getInventory();
	// Check the player has all the resources required
	for (auto res : cr.requiredResources) {
		if (inv[res.first] < res.second) {
			return;
		}
	}
	std::shared_ptr<Entity> e = cr.createMethod(this->game, pos);
	// Check the entity is valid
	auto x = e->components.transform->getPosition();
	if (this->ensureValid(e)) {
		// Add the thingy
		this->game->addEntity(e);
		// Remove the required resources
		for (auto res : cr.requiredResources) {
			player->components.inventory->removeItems(res.first, res.second);
		}
	}
}
bool GameHud::ensureValid(std::shared_ptr<Entity> e) {
	if (e->tag == EntityTag::Base) {
		// Ensure the base is on the land
		sf::Vector2i pos = (sf::Vector2i)(e->components.transform->getPosition() / 64.0f);
		for (size_t x = 0; x < 3; x++) {
			for (size_t y = 0; y < 3; y++) {
				if (this->game->getGameMap()->getTileAt(pos.x + x, pos.y + y) == GameMap::TileType::Sea) {
					return false;
				}
			}
		}
		// If the base is a mining/forestry/something base, make sure it's on the right resource
		// Check it's a generation base
		if (auto cont = std::dynamic_pointer_cast<MiningBaseController>(e->components.controller)) {
			// Get the resource
			GameResource neededRes = cont->getResource();
			bool hasResource = false;
			// Check the resource is present
			for (std::shared_ptr<Entity> other : this->game->getEntities()) {
				// Check the entity is a resource and is the correct resource
				if (auto otherCont = std::dynamic_pointer_cast<ResourceController>(other->components.controller)) {
					if (otherCont->getResource() == neededRes) {
						sf::Vector2i otherPos = sf::Vector2i(other->components.transform->getPosition()) / 64;
						if (otherPos.x - pos.x < 3 && otherPos.x - pos.x >= 0) {
							if (otherPos.y - pos.y < 3 && otherPos.y - pos.y >= 0) {
								hasResource = true;
								break;
							}
						}
					}
				}
			}
			if (!hasResource)
				return false;

			for (size_t x = 0; x < 3; x++) {
				for (size_t y = 0; y < 3; y++) {
				}
			}
		}
		return true;
	}
	// TBA
	return true;
}
void GameHud::transferItems(std::weak_ptr<Entity> e, GameResource res, unsigned int amount) {
	this->selectCallback = std::bind([&](std::weak_ptr<Entity> to, std::weak_ptr<Entity> from) {
		// TODO, make sure entities are close to each other
		if (to.lock() && from.lock()) {
			std::map<GameResource, unsigned int> resources = from.lock()->components.inventory->getInventory();
			// Remove resources from entity one
			for (auto it : resources) {
				from.lock()->components.inventory->removeItems(it.first, it.second);
			}
			// Add resources to entity two
			for (auto it : resources) {
				to.lock()->components.inventory->addItems(it.first, it.second);
			}
		}
	}, std::placeholders::_1, e);
	this->currentClickState = ClickState::Selecting;
}
void GameHud::onClick(sf::Vector2f pos) {
	if (this->currentClickState == ClickState::Nothing) {
		for (auto e : this->game->getEntities()) {
			if (e->components.click != nullptr) {
				if (e->components.click->checkForClick(pos)) {
					if (e->components.controller != nullptr) {
						e->components.controller->onClick();
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
			if (e->components.click) {
				if (e->components.click->checkForClick(pos)) {
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
	// Hide the buttons if they are showing
	if (this->buildGroup->isVisible()) {
		this->buildGroup->setVisible(false);
	}
	// Show the buttons
	else {
		this->buildGroup->setVisible(true);
		this->resetBuildButtons();
	}
}
// Reset the buttons inside researchGroup
void GameHud::resetResearchButtons() {
	// Remove all current buttons
	this->researchGroup->removeAllWidgets();
	// The y coordinate of the next button to be added
	// Used to prevent the buttons from overlapping
	float y = this->researchButton->getFullSize().y;
	// Add a button for each technology that is unresearched and is able to be researched
	for (auto it : TechTree::nodes) {
		if (it.second.parent == Technology::Nothing
			|| TechTree::nodes.find(it.second.parent)->second.isResearched) {
			// Create a button for it
			auto btn = tgui::Button::create();
			if (it.second.isResearched) {
				btn->setEnabled(false);
			}
			btn->setText(it.second.name);
			btn->setPosition({ 500.0f, y });
			btn->connect("clicked", [&](Game* g, Technology tech) {
				// Check the player has the correct resources for the technology
				TechTreeNode* node = &TechTree::nodes.find(tech)->second;
				std::shared_ptr<Entity> player = g->getPlayer();
				std::map<GameResource, unsigned int> playerInventory = player->components.inventory->getInventory();
				// Check the player has enough money
				if (this->game->getMoney() < node->cost) {
					return false;
				}
				// Remove the money
				this->game->removeMoney(node->cost);
				// Set the node to researched
				node->isResearched = true;
				// Reset the buttons
				g->getHud()->resetBuildButtons();
				g->getHud()->resetResearchButtons();
			}, this->game, it.first);
			// Add the button to the group
			this->researchGroup->add(btn);
			y += btn->getFullSize().y;
		}
	}
}
void GameHud::resetBuildButtons() {
	// Add the build buttons
	float y = 0.0f;
	this->buildGroup->removeAllWidgets();
	// Vector of buttons of stuff to build
	for (auto cr : CraftingRecipes::recipes) {
		// Make sure the user can build this thing
		if (cr.requiredTech == Technology::Nothing || TechTree::nodes[cr.requiredTech].isResearched) {
			// Add the actual button
			tgui::Button::Ptr btn = tgui::Button::create();
			btn->setText(cr.displayText);
			btn->connect("clicked", [&](Game* g, CraftingRecipes::CraftRecipe cr) {
				g->getHud()->selectPoint(ClickState::Building, std::bind(
					[&](Game* g, sf::Vector2f pos, CraftingRecipes::CraftRecipe cr) {
					g->getHud()->tryToBuild(cr, pos);
				}, std::placeholders::_1, std::placeholders::_2, cr));
			}, this->game, cr);
			btn->setPosition({ 0, y });
			this->buildGroup->add(btn);
			y += btn->getFullSize().y;
		}
	}
}
void GameHud::toggleResearchButtons() {
	if (this->researchGroup->isVisible()) {
		this->researchGroup->setVisible(false);
	}
	else {
		this->researchGroup->setVisible(true);
		this->resetResearchButtons();
	}
}
