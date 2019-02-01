#include "GameHud.h"
#include "MiningBaseController.h"
#include "EntityPrefabs.h"
#include "CraftRecipes.h"
#include "TechTree.h"
#include "InventoryComponent.h"
#include "ResourceController.h"
#include "GameResource.h"
#include "BasicTransform.h"

const float GameHud::ANNOUNCEMENT_WIDTH = 400.0f;
const float GameHud::ANNOUNCEMENT_ITEM_HEIGHT = 60.0f;

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
	this->announcementContainer = tgui::VerticalLayout::create();
	this->announcementContainer->setPosition({ this->game->getGui()->getSize().x - 400, 500 });
	this->game->getGui()->add(this->announcementContainer);
	// Create entity panel
	this->entityPanel = tgui::Panel::create();
	this->entityPanel->setSize(tgui::bindWidth(this->game->getGui()), 250);
	this->entityPanel->setPosition(
		0,
		tgui::bindHeight(this->game->getGui()) - tgui::bindHeight(this->entityPanel)
	);
	this->game->getGui()->add(this->entityPanel);
}
void GameHud::update() {
	// Update money display
	this->moneyDisplay->setText("$" + std::to_string(this->game->getMoney()));
	// Update player health
	this->playerHealth->setValue(this->game->getPlayer()->components.health->getHealth());
	// Update announcements
	if (!this->announcements.empty()) {
		while (this->announcements.front().second.getElapsedTime().asMilliseconds() >= 3000.0) {
			this->announcementContainer->remove(this->announcements.front().first);
			this->announcements.pop();
			this->announcementContainer->setSize(
				this->announcementContainer->getSize().x,
				this->announcements.size() * ANNOUNCEMENT_ITEM_HEIGHT);
		}
	}
}
void GameHud::render(RenderManager& rm) {
	if (this->currentClickState == ClickState::Building && this->toBuild) {
		// Move to mouse coords, rounded to 64px
		sf::Vector2i pos = sf::Vector2i(this->game->getMouseCoords() / 64.0f) * 64;
		this->toBuild->components.transform->setPosition(sf::Vector2f(pos));
		// Reset the texture
		this->toBuild->components.renderer->reset();
		// Render
		this->toBuild->components.renderer->render(rm);
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
			std::weak_ptr<Entity> resSource;
			if (generationBaseNeedsSource(neededRes)) {
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
									resSource = other;
									break;
								}
							}
						}
					}
				}
				if (!hasResource) {
					return false;
				}
				// Remove the source
				this->game->removeEntity(resSource);
			}
		}
		return true;
	}
	// TBA
	return true;
}
void GameHud::transferItems(std::weak_ptr<Entity> e, GameResource res, unsigned int amount) {
	this->selectCallback = std::bind([&](
		std::weak_ptr<Entity> to,
		std::weak_ptr<Entity> from,
		GameResource res,
		unsigned int amount) {
		// Ensure both entities still exist
		if (to.lock() && from.lock()) {
			// Remove resources
			from.lock()->components.inventory->removeItems(res, amount);
			// Add resources
			to.lock()->components.inventory->addItems(res, amount);
		}
	}, std::placeholders::_1, e, res, amount);
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
		// Remove the money required for the entity
		std::map<GameResource, unsigned int> playerInv = this->game->getPlayer()->components.inventory->getInventory();
		std::map<GameResource, unsigned int> neededInv = this->buildRecipe.requiredResources;
		for (auto it = neededInv.begin(); it != neededInv.end(); it++) {
			if (playerInv[it->first] < it->second) {
				// Todo: tell the player they don't have the stuff to build the entity
				this->currentClickState = Nothing;
				return;
			}
		}
		// Remove the inventory from the player
		for (auto it = neededInv.begin(); it != neededInv.end(); it++) {
			this->game->getPlayer()->components.inventory->removeItems(it->first, it->second);
		}
		// Create the entity
		std::shared_ptr<Entity> e = this->buildRecipe.createMethod(this->game, this->game->getMouseCoords());
		// Check if it works
		if (this->ensureValid(e)) {
			// Add it to the game
			this->game->addEntity(e);
			this->currentClickState = ClickState::Nothing;
		}
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
	for (auto it : this->game->getTechTree()->nodes) {
		if (it.second.parent == Technology::Nothing
			|| this->game->getTechTree()->nodes[it.second.parent].isResearched) {
			// Create a button for it
			auto btn = tgui::Button::create();
			if (it.second.isResearched) {
				btn->setEnabled(false);
			}
			btn->setText(it.second.name);
			btn->setPosition({ 500.0f, y });
			btn->connect("clicked", [=](Game* g, Technology tech) {
				// Check the player has the correct resources for the technology
				TechTreeNode* node = &this->game->getTechTree()->nodes.find(tech)->second;
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
				return true;
			}, this->game, it.first);
			// Make the tool tip have information about the technology
			tgui::TextBox::Ptr toolTip = tgui::TextBox::create();
			toolTip->setPosition({ 10, 10 });
			toolTip->setText(it.second.name + ": $" + std::to_string(it.second.cost) + "\n");
			toolTip->addText("-------------\n\n");
			toolTip->addText(it.second.description + "\n\n");
			toolTip->addText("Allows researching:\n");
			toolTip->addText("-------------");
			// Get all the technologies that this technology unlocks
			std::vector<std::string> allowsResearching;
			for (auto sIt = this->game->getTechTree()->nodes.begin(); sIt != this->game->getTechTree()->nodes.end(); sIt++) {
				if (sIt->second.parent == it.first) {
					allowsResearching.push_back(sIt->second.name);
				}
			}
			// Add them to toolTip
			for (auto s : allowsResearching) {
				toolTip->addText("\n" + s);
			}
			// Set tool tip
			btn->setToolTip(toolTip);
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
		if (cr.requiredTech == Technology::Nothing || this->game->getTechTree()->nodes[cr.requiredTech].isResearched) {
			// Add the actual button
			tgui::Button::Ptr btn = tgui::Button::create();
			btn->setText(cr.displayText);
			tgui::TextBox::Ptr tt = tgui::TextBox::create();
			btn->setToolTip(tt);
			std::string resourceText;
			for (auto it = cr.requiredResources.begin(); it != cr.requiredResources.end(); it++) {
				resourceText += getResourceString(it->first) + ": " + std::to_string(it->second) + "\n";
			}
			tt->setText(cr.displayText + "\n"
				+ "-------------------\n\n"
				+ "Required Resources:\n"
				+ "-------------------\n"
				+ resourceText
			);
			btn->connect("clicked", [&](Game* g, CraftingRecipes::CraftRecipe cr) {
				g->getHud()->chooseEntityToBuild(cr);
			}, this->game, cr);
			btn->setPosition({ 0, y });
			this->buildGroup->add(btn);
			y += btn->getFullSize().y;
		}
	}
}
void GameHud::chooseEntityToBuild(CraftingRecipes::CraftRecipe cr) {
	// Create the entity
	std::shared_ptr<Entity> e = cr.createMethod(this->game, { 0.0f, 0.0f });
	// Overwrite all their components except for renderer
	// Set all to nullptr except transform, which is set to simple transform
	auto x = e->components.transform.use_count();
	std::shared_ptr<RenderComponent> r = e->components.renderer;
	e->components = ComponentList(
		new BasicTransform({ 0.0f, 0.0f }, 0.0f),
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	);
	e->components.renderer = r;
	// Save this modified entity to be used to draw for a hovering effect
	this->toBuild = e;
	// Save the method to build the entity
	this->buildRecipe = cr;
	// Set click state
	this->currentClickState = ClickState::Building;
}
void GameHud::buildEntity() {
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
void GameHud::addAnnouncement(std::string announcement) {
	tgui::TextBox::Ptr p = tgui::TextBox::create();
	p->setText(announcement);
	p->getRenderer()->setTextColor(tgui::Color(rand() % 255, rand() % 255, rand() % 255, 255));
	this->announcementContainer->add(p);
	this->announcements.push({ p, sf::Clock() });
	this->announcementContainer->setSize(
		ANNOUNCEMENT_WIDTH,
		this->announcements.size() * ANNOUNCEMENT_ITEM_HEIGHT);
}
tgui::Panel::Ptr GameHud::getEntityPanel() {
	return this->entityPanel;
}