#include "GuiComponent.h"
#include "InventoryComponent.h"
#include "ControllerComponent.h"
#include <TGUI/Gui.hpp>

const float GuiComponent::WINDOW_HEIGHT = 500.0f;
const float GuiComponent::WINDOW_WIDTH = 600.0f;

void GuiComponent::setParent(std::weak_ptr<Entity> parent) {
	Component::setParent(parent);
	// Create window
	this->entityWindow = tgui::ChildWindow::create();
	this->entityWindow->setSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	// Add all component's panels to it
	auto p = tgui::ScrollablePanel::create();
	this->entityWindow->add(p);
	tgui::Widget::Ptr last;
	for (ComponentType c : ComponentList::allTypes) {
		if (this->getParent().lock()->components.get(c)) {
			// Get the GUI (may be nullptr)
			tgui::Widget::Ptr w = this->getParent().lock()->components.get(c)->getGui();
			if (w) {
				// Set position under last widget
				if (last)
					w->setPosition(0, tgui::bindTop(last) + tgui::bindHeight(last));
				else
					w->setPosition(0, 0);
				// Add it
				p->add(w);
				last = w;
			}
		}
	}
}
void GuiComponent::show() {
	auto eP = this->getGame()->getHud()->getEntityPanel();
	eP->removeAllWidgets();
	// Create a group for the general entity info panel
	auto info = tgui::Group::create();
	info->setPosition(0, 20);
	info->setSize(tgui::bindWidth(eP) / 5, tgui::bindHeight(eP));
	eP->add(info);
	// Add entity name
	auto name = tgui::Label::create();
	info->add(name);
	name->setText(this->getParent().lock()->getStringRep());
	// Create a container for the controller/inventory components
	auto container = tgui::HorizontalLayout::create();
	container->setSize(tgui::bindWidth(eP) - tgui::bindWidth(info), tgui::bindHeight(eP) - 20);
	container->setPosition(tgui::bindWidth(info), 20);
	// Add the controller's gui
	if (auto c = this->getParent().lock()->components.controller) {
		if (auto w = c->getGui()) {
			container->add(w);
		}
	}
	// Add the inventory's gui
	if (auto i = this->getParent().lock()->components.inventory) {
		if (auto w = i->getGui()) {
			container->add(w);
		}
	}
	eP->add(container);
	this->getGame()->getGui()->add(eP);
}
void GuiComponent::hide() {
	this->getGame()->getGui()->remove(this->getGame()->getHud()->getEntityPanel());
}