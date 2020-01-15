#pragma once
#include "RenderComponent.h"
class ResourceRenderer : public RenderComponent {
public:
	ResourceRenderer(GameResource res);
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void render(RenderManager& rm) override;
	virtual void reset() override;
private:
	sf::Sprite spr;
};