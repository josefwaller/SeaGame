#include "ClickComponent.h"
#include "Box2dTransform.h"

ClickComponent::ClickComponent(std::weak_ptr<Entity> parent) : Component(parent) {
}