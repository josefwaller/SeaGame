#pragma once
#include <TGUI/Container.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/ProgressBar.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>
#include <queue>
#include <string>
// Forward declarations
class Game;
struct Entity;

/*
 * The default screen, allows the player to click on an entity and shows that entity's information
 */
class DefaultScreen {
public:
	// The width of the announcement widget
	static const float ANNOUNCEMENT_WIDTH;
	// The height of each announcement
	static const float ANNOUNCEMENT_ITEM_HEIGHT;
	DefaultScreen(Game* g);
	void onClick(sf::Vector2f mousePos);
	void update();
	void show(tgui::Container::Ptr c);
	void hide(tgui::Container::Ptr c);
	// Show an entity via entityPanel
	void showEntity(std::weak_ptr<Entity> e);
	// Add an announcement
	void addAnnoucnement(std::string announcement);
private:
	Game* game;
	// The container of everything, for adding/removing
	tgui::Container::Ptr container;
	// The container containing the announcements
	tgui::Container::Ptr announcementContainer;
	// The panel used to show the entity information
	tgui::Container::Ptr entityPanel;
	// The label used to display the user's amount of money
	tgui::Label::Ptr moneyDisplay;
	// The player's health display
	tgui::ProgressBar::Ptr playerHealth;
	// The announcements being shown
	// Hold the GUI component and the clock in order to check when to destory it
	std::queue<std::pair<tgui::Widget::Ptr, sf::Clock>> announcements;
};
