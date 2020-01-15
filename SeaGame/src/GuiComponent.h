#pragma once
#include "Component.h"
#include "GameResource.h"
#include <TGUI/Gui.hpp>
#include <TGUI/Container.hpp>

class GuiComponent : public Component {
public:
	// Width of the entity's window
	static const float WINDOW_WIDTH;
	// Height of the entity's window
	static const float WINDOW_HEIGHT;
	// Show the entity's GUI on the screen
	void show(tgui::Container::Ptr c);
	// Hide the entity's GUI
	void hide(tgui::Container::Ptr c);
};