#include "GuiComponent.h"
#include "Box2dTransform.h"
#include "FerryShipController.h"
#include "MiningBaseController.h"

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
	// Add all of the panels to the game hud's entityPanel
	auto container = tgui::HorizontalLayout::create();
	this->getGame()->getHud()->getEntityPanel()->add(container);
	for (ComponentType c : ComponentList::allTypes) {
		// If the entity has this component
		std::shared_ptr<Component> comp = this->getParent().lock()->components.get(c);
		if (c) {
			// If the component has a GUI
			tgui::Widget::Ptr w = comp->getGui();
			if (w) {
				container->add(w);
			}
		}
	}
}
void GuiComponent::hide() {
	this->getGame()->getGui()->remove(this->entityWindow);
}