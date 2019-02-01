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
	// Show the entity's GUI on the screen
	void show();
	// Hide the entity's GUI
	void hide();
private:
	// The window which shows the data for this entity
	tgui::ChildWindow::Ptr entityWindow;
};