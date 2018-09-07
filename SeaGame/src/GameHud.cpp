#include "GameHud.h"
#include "EntityPrefabs.h"

GameHud::GameHud() {
	auto x = 0;
}
GameHud::GameHud(Game* g) {
	this->game = g;
	// Add the build button
	this->buildButton = tgui::Button::create();
	this->buildButton->setText("Build");
	this->game->getGui().add(this->buildButton);
	this->buildButton->connect("clicked", &GameHud::toggleBuildButtons, game->getHud());
	this->currentClickState = ClickState::Nothing;
	float width = 0.0f;
	float height = this->buildButton->getFullSize().y;
	this->toBuildButtons = {
		tgui::Button::create(),
		tgui::Button::create(),
		tgui::Button::create()
	};
	this->toBuildButtons[0]->setText("Generation Base");
	this->toBuildButtons[0]->setPosition({ 0, this->buildButton->getFullSize().y });
	this->toBuildButtons[0]->connect("clicked", [&](GameHud* gh) {
		gh->selectPoint(ClickState::Building, [&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::miningBase(g, (sf::Vector2i)(pos / 64.0f)));
		});
	}, game->getHud());
	width += this->toBuildButtons[0]->getFullSize().x;
	this->toBuildButtons[1]->setText("Military Base");
	this->toBuildButtons[1]->setPosition({ width, height });
	this->toBuildButtons[1]->connect("clicked", [&](GameHud * gh) {
		gh->selectPoint(ClickState::Building, [&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::militaryBase(g, (sf::Vector2i)(pos / 64.0f)));
		});
	}, game->getHud());
	width += this->toBuildButtons[1]->getFullSize().x;
	this->toBuildButtons[2]->setText("Ferry Ship");
	this->toBuildButtons[2]->setPosition({ width, height });
	this->toBuildButtons[2]->connect("clicked", [&](GameHud* gh) {
		gh->selectPoint(ClickState::Building, [&](Game* g, sf::Vector2f pos) {
			g->addEntity(EntityPrefabs::ferryShip(g, pos, {}, {}));
		});
	}, game->getHud());

}
void GameHud::onClick(sf::Vector2f pos) {
	if (this->currentClickState == ClickState::Nothing) {
		for (auto e : this->game->getEntities()) {
			if (e->gui != nullptr) {
				if (e->gui->checkForClick(pos)) {
					break;
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
			if (e->gui) {
				if (e->gui->checkForClick(pos)) {
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
	for (auto btn : this->toBuildButtons) {
		this->game->getGui().add(btn);
	}
}
