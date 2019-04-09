#include "BuildScreen.h"
#include "CraftRecipes.h"
#include "Game.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "InventoryComponent.h"
#include "BasicTransform.h"
#include "ResourceController.h"
#include "RenderComponent.h"
#include "GenerationBaseController.h"
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/SignalImpl.hpp>

BuildScreen::BuildScreen() {}
BuildScreen::BuildScreen(Game* g) {
	this->game = g;
	this->isBuilding = false;
	this->buttons = tgui::Group::create();
	this->update();
}
void BuildScreen::update() {
	// Add the buttons
	this->buttons->removeAllWidgets();
	float y = 30.0f;
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
			btn->connect("clicked", [&](BuildScreen* b, CraftingRecipes::CraftRecipe cr) {
				b->setToBuild(cr);
			}, this, cr);
			btn->setPosition({ 0, y });
			this->buttons->add(btn);
			y += btn->getFullSize().y;
		}
	}
	// Add the close button
	auto closeBtn = tgui::Button::create();
	closeBtn->setText("Close");
	closeBtn->connect("clicked", [&](Game* g) {
		g->getHud()->hideBuild();
	}, this->game);
	closeBtn->setPosition(sf::Vector2f(
		(this->game->getWindow()->getSize().x - closeBtn->getSize().x) / 2,
		30
	));
	this->buttons->add(closeBtn);
}
void BuildScreen::setToBuild(CraftingRecipes::CraftRecipe cr) {
	// Set the sprite to display
	this->buildSprite = cr.displaySprite;
	// Save the method to build the entity
	this->buildingRecipe = cr;
	this->isBuilding = true;
}
void BuildScreen::onClick(sf::Vector2f pos) {
	if (this->isBuilding) {
		// Remove the money required for the entity
		std::map<GameResource, unsigned int> playerInv = this->game->getPlayer()->components.inventory->getInventory();
		std::map<GameResource, unsigned int> neededInv = this->buildingRecipe.requiredResources;
		for (auto it = neededInv.begin(); it != neededInv.end(); it++) {
			if (playerInv[it->first] < it->second) {
				// Todo: tell the player they don't have the stuff to build the entity
				return;
			}
		}
		// Remove the inventory from the player
		for (auto it = neededInv.begin(); it != neededInv.end(); it++) {
			this->game->getPlayer()->components.inventory->removeItems(it->first, it->second);
		}
		// Create the entity
		auto a = this->game->getMouseCoords();
		auto b = a / 64.0f;
		auto c = sf::Vector2i(b);
		auto d = sf::Vector2f(c);
		std::shared_ptr<Entity> e = this->buildingRecipe.createMethod(this->game,
			d * 64.0f
		);
		// Check if it works
		if (this->ensureValid(e)) {
			// Add it to the game
			this->game->addEntity(e);
			this->isBuilding = false;
		}
	}
}
void BuildScreen::render(RenderManager& rm) {
	if (this->isBuilding) {
		this->showCurrentBuild(rm);
	}
}
void BuildScreen::showCurrentBuild(RenderManager& rm) {
	// Move to mouse coords, rounded to 64px
	sf::Vector2i pos = sf::Vector2i(this->game->getMouseCoords() / 64.0f) * 64;
	this->buildSprite.setPosition(sf::Vector2f(pos));
	rm.add(this->buildSprite, RenderManager::INDEX_DECK);
	// Tell the player if the location is invalid
	/*if (!ensureValid(this->toBuild)) {
		sf::Sprite invalidSprite = ResourceManager::get()->getSprite(
			"medievalRTS_spritesheet@2",
			"medievalStructure_14.png",
			true
		);
		invalidSprite.setPosition(toBuild->components.transform->getPosition());
		rm.add(invalidSprite, RenderManager::INDEX_DEBUG);
	}*/
}
bool BuildScreen::ensureValid(std::shared_ptr<Entity> e) {
	if (e->tag == EntityTag::Base) {
		// Ensure the base is on the land
		auto a = e->components.transform->getPosition();
		sf::Vector2i pos = sf::Vector2i(a / 64.0f);
		for (size_t x = 0; x < 3; x++) {
			for (size_t y = 0; y < 3; y++) {
				if (this->game->getGameMap()->getTileAt(pos.x + x, pos.y + y) == GameMap::TileType::Sea) {
					return false;
				}
			}
		}
		// If the base is a mining/forestry/something base, make sure it's on the right resource
		// Check it's a generation base
		if (auto cont = std::dynamic_pointer_cast<GenerationBaseController>(e->components.controller)) {
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

void BuildScreen::show(tgui::Container::Ptr c) {
	c->add(this->buttons);
}
void BuildScreen::hide(tgui::Container::Ptr c) {
	c->remove(this->buttons);
}