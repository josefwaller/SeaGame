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
	// Set position to the entity's position
	this->getGame()->getGui()->add(this->entityWindow);
	this->entityWindow->setPosition(
		sf::Vector2f(this->getGame()->getWindow()->mapCoordsToPixel(
			this->getParent().lock()->components.transform->getPosition()
		))
	);
}
void GuiComponent::hide() {
	this->getGame()->getGui()->remove(this->entityWindow);
}