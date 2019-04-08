#include "ResearchScreen.h"
#include "TechTree.h"
#include "Game.h"
#include "RenderManager.h"
#include <TGUI/SignalImpl.hpp>

ResearchScreen::ResearchScreen() {}
ResearchScreen::ResearchScreen(Game* g) {
	this->game = g;
	this->researchBtns = tgui::ScrollablePanel::create();
	// Make the panel have a transparent background
	this->researchBtns->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 100));
	// How much to move overthe buttons in each "column" of technologies
	// Basically, the distance between a node and its child
	const float LAYER_WIDTH = 600.0f;
	// The spatial height of the tree, i.e. how tall a column should be
	const float COLUMN_HEIGHT = 900.0f;
	TechTree* techTree = this->game->getTechTree();
	// Get all the root techs
	std::vector<Technology> currentTechs;
	for (auto it = techTree->nodes.begin(); it != techTree->nodes.end(); it++) {
		if (it->second.parent == Technology::Nothing) {
			currentTechs.push_back(it->first);
		}
	}
	float x = 0.0f;
	while (!currentTechs.empty()) {
		std::vector<Technology> nextTechs;
		// The vertical distance between technologies
		const float RESEARCH_HEIGHT = 200.0f;
		// Easy reference to the center of the screen
		const float CENTER_LINE = this->game->getWindow()->getSize().y / 2.0f;
		// The highest y position a technology will be in this columnb
		const float TOP_LIMIT = CENTER_LINE - RESEARCH_HEIGHT * (float)(currentTechs.size() - 1) / 2.0f;
		// Add a button for each tech
		for (auto it = currentTechs.begin(); it != currentTechs.end(); it++) {
			auto btn = getButtonForTech(*it);
			this->techButtons[*it] = btn;
			size_t offset = it - currentTechs.begin();
			btn->setPosition(sf::Vector2f(
				x,
				TOP_LIMIT + RESEARCH_HEIGHT * (float)offset
			));
			this->researchBtns->add(btn);
			// Get the next techs to add
			for (auto cIt = techTree->nodes.begin(); cIt != techTree->nodes.end(); cIt++) {
				if (cIt->second.parent == *it) {
					nextTechs.push_back(cIt->first);
				}
			}
		}
		// Go to the next column of technologies
		currentTechs = nextTechs;
		x += LAYER_WIDTH;
	}
	// Create the lines between technologies
	this->techLines = sf::VertexArray(sf::Lines, 2 * techTree->nodes.size());
	size_t index = 0;
	for (auto it = techTree->nodes.begin(); it != techTree->nodes.end(); it++) {
		if (it->second.parent == Technology::Nothing) {
			continue;
		}
		// Get the parent button
		auto parentButton = this->techButtons[it->second.parent];
		// Get the child button
		auto childButton = this->techButtons[it->first];
		// Get the coords to draw the line to
		sf::Vector2f fromCoord(
			parentButton->getAbsolutePosition().x + parentButton->getSize().x,
			parentButton->getAbsolutePosition().y + parentButton->getSize().y / 2.0f
		);
		sf::Vector2f toCoord(
			childButton->getAbsolutePosition().x,
			childButton->getAbsolutePosition().y + childButton->getSize().y / 2.0f
		);
		// Create the line
		sf::Color color = sf::Color::Red;
		sf::Vertex fromVertex(fromCoord, color, sf::Vector2f());
		sf::Vertex toVertex(toCoord, color, sf::Vector2f());
		// Add to lines
		this->techLines[2 * index] = fromVertex;
		this->techLines[2 * index + 1] = toVertex;
		index++;
	}
	// Add the close button
	this->closeBtn = tgui::Button::create();
	this->researchBtns->add(this->closeBtn);
	this->closeBtn->connect("clicked", [&](GameHud* gH) {
		gH->hideResearch();
	}, this->game->getHud());
	this->closeBtn->setText("Close Research Screen");
	this->closeBtn->setPosition(
		(this->game->getWindow()->getSize().x - this->closeBtn->getSize().x) / 2,
		0.0f
	);
	this->update();
}
void ResearchScreen::update() {
	// Get the tree
	TechTree* t = this->game->getTechTree();
	// Set each button to enabled or disabled depending on whether the player can research it
	for (auto it = t->nodes.begin(); it != t->nodes.end(); it++) {
		// Get whether the technology is researched
		bool isResearched = it->second.isResearched;
		// Get whether the technology's parent is researched
		bool parentResearched = it->second.parent == Technology::Nothing || t->nodes[it->second.parent].isResearched;
		if (parentResearched && !isResearched) {
			this->techButtons[it->first]->setEnabled(true);
		}
		else {
			this->techButtons[it->first]->setEnabled(false);
		}
	}
}
void ResearchScreen::render(RenderManager& rm) {
	rm.add(this->techLines, RenderManager::INDEX_EFFECT);
}
tgui::Button::Ptr ResearchScreen::getButtonForTech(Technology t) {
	TechTreeNode node = this->game->getTechTree()->nodes[t];
	// Create new button
	auto btn = tgui::Button::create();
	// Set enabled if not researched
	if (node.isResearched) {
		btn->setEnabled(false);
	}
	// Set text
	btn->setText(node.name);
	// Set on click
	btn->connect("clicked", [=](Game* g, Technology tech) {
		// Check the player has the correct resources for the technology
		TechTreeNode* node = &g->getTechTree()->nodes.find(tech)->second;
		std::shared_ptr<Entity> player = g->getPlayer();
		// Check the player has enough money
		if (g->getMoney() < node->cost) {
			return false;
		}
		// Remove the money
		g->removeMoney(node->cost);
		// Set the node to researched
		node->isResearched = true;
		// Reset the buttons
		g->getHud()->updateBuild();
		return true;
	}, this->game, t);
	// Make the tool tip have information about the technology
	tgui::TextBox::Ptr toolTip = tgui::TextBox::create();
	toolTip->setPosition({ 10, 10 });
	toolTip->setText(node.name + ": $" + std::to_string(node.cost) + "\n");
	toolTip->addText("-------------\n\n");
	toolTip->addText(node.description + "\n\n");
	toolTip->addText("Allows researching:\n");
	toolTip->addText("-------------");
	// Get all the technologies that this technology unlocks
	std::vector<std::string> allowsResearching;
	TechTree* techTree = this->game->getTechTree();
	for (auto it = techTree->nodes.begin(); it != techTree->nodes.end(); it++) {
		if (it->second.parent == t) {
			allowsResearching.push_back(it->second.name);
		}
	}
	// Add them to toolTip
	for (auto s : allowsResearching) {
		toolTip->addText("\n" + s);
	}
	// Set tool tip
	btn->setToolTip(toolTip);
	return btn;
}
void ResearchScreen::show(tgui::Container::Ptr c) {
	c->add(this->researchBtns);
}
void ResearchScreen::hide(tgui::Container::Ptr c) {
	c->remove(this->researchBtns);
}
