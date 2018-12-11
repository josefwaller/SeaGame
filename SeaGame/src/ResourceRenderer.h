#pragma once
#include "RenderComponent.h"
class ResourceRenderer : public RenderComponent {
public:
	ResourceRenderer(std::weak_ptr<Entity> parent, GameResource res);
	virtual void render(RenderManager& rm) override;
private:
	sf::Sprite spr;
};