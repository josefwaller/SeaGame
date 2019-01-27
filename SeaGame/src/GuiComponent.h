#pragma once
#include "Component.h"
#include "GameResource.h"

class GuiComponent : public Component {
public:
	// Width of the entity's window
	static const float WINDOW_WIDTH;
	// Height of the entity's window
	static const float WINDOW_HEIGHT;
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	// Change the panel the component is currently showing
	void changePanel(std::string selectedPanel);
	std::string getResourceString(GameResource res);
	void show();
	void hide();
	// Update the info in on the gui panels
	void update();
private:
	void onClick();
	// The window which shows the data for this entity
	tgui::ChildWindow::Ptr entityWindow;
	// Tabs for each kind of gui needed to be shown
	// i.e. inventory, health, etc
	tgui::Tabs::Ptr entityTabs;
	// The currently selected panel
	std::string selectedPanel;
	// The panels in the window
	std::map<std::string, tgui::Panel::Ptr> entityPanels;
	// Group that holds the panels in the window
	tgui::Group::Ptr panelGroup;
};