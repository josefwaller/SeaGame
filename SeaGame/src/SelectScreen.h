#pragma once
#include <TGUI/Container.hpp>
#include <TGUI/Widget.hpp>
#include <memory>
#include <functional>
// Forward Declarations
class Entity;
class Game;

class SelectScreen {
public:
	SelectScreen(Game* g);
	void onClick(sf::Vector2f pos);
	void show(tgui::Container::Ptr c);
	void hide(tgui::Container::Ptr c);
	void setCallback(std::function<void(std::shared_ptr<Entity>)> c);
private:
	Game* game;
	std::function<void(std::shared_ptr<Entity>)> callback;
	// The text that says "Select an entity"
	tgui::Widget::Ptr label;
};
