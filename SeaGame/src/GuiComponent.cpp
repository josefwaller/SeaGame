#include "GuiComponent.h"
#include "InventoryComponent.h"
#include "ControllerComponent.h"
#include "ComponentList.h"
#include "Entity.h"
#include <TGUI/Gui.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>

const float GuiComponent::WINDOW_HEIGHT = 500.0f;
const float GuiComponent::WINDOW_WIDTH = 600.0f;

void GuiComponent::show(tgui::Container::Ptr c) {
	// Create a group for the general entity info panel
	auto info = tgui::Group::create();
	info->setPosition(0, 20);
	info->setSize(tgui::bindWidth(c) / 5, tgui::bindHeight(c));
	c->add(info);
	// Add entity name
	auto name = tgui::Label::create();
	info->add(name);
	name->setText(this->getParent().lock()->getStringRep());
	// Create a container for the controller/inventory components
	auto container = tgui::HorizontalLayout::create();
	container->setSize(tgui::bindWidth(c) - tgui::bindWidth(info), tgui::bindHeight(c) - 20);
	container->setPosition(tgui::bindWidth(info), 20);
	// Add the controller's gui
	if (auto c = this->getComponentList().controller) {
		if (auto w = c->getGui()) {
			container->add(w);
		}
	}
	// Add the inventory's gui
	if (auto i = this->getComponentList().inventory) {
		if (auto w = i->getGui()) {
			container->add(w);
		}
	}
	c->add(container);
}
void GuiComponent::hide(tgui::Container::Ptr c) {
//	this->getGame()->getGui()->remove(this->getGame()->getHud()->getEntityPanel());
}