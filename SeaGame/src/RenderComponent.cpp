#include "RenderComponent.h"

RenderComponent::RenderComponent(std::weak_ptr<Entity> newParent): Component(newParent)
{
}

void RenderComponent::renderCollider(RenderManager& r)
{
}
void RenderComponent::reset()
{

}