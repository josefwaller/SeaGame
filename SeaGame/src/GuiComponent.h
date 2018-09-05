#pragma once
#include "Component.h"
#include "GameResource.h"

class GuiComponent : public Component {
public:
	GuiComponent(std::shared_ptr<Entity>);
	void checkForClick(sf::Vector2f mousePos);
	// Change the panel the component is currently showing
	void changePanel(std::string selectedPanel);
	void updateInventory();
	std::string getResourceString(GameResource res);
private:
	void onClick();
	// The window which shows the data for this entity
	tgui::ChildWindow::Ptr entityWindow;
	// Tabs for each kind of gui needed to be shown
	// i.e. inventory, health, etc
	tgui::Tabs::Ptr entityTabs;
	// The currently selected panel
	std::string selectedPanel;
	std::map<std::string, tgui::Panel::Ptr> entityPanels;
};