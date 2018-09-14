#pragma once
#include "BaseRenderer.h"

class MiningBaseRenderer : public BaseRenderer {
public:
	MiningBaseRenderer(std::weak_ptr<Entity> parent, GameResource res);
	virtual void render(RenderManager& rm) override;
private:
	sf::Sprite resSprite;
	static sf::Sprite getResourceSprite(GameResource res);
};