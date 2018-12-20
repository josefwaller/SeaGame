#pragma once
#include "BaseRenderer.h"

class MiningBaseRenderer : public BaseRenderer {
public:
	MiningBaseRenderer(GameResource res);
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void render(RenderManager& rm) override;
private:
	sf::Sprite resSprite;
};