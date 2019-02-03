#pragma once
#include "BaseRenderer.h"
#include "GameResource.h"

class MiningBaseRenderer : public BaseRenderer {
public:
	MiningBaseRenderer(GameResource res);
	virtual void setParent(std::weak_ptr<Entity> parent) override;
	virtual void render(RenderManager& rm) override;
	virtual void reset() override;
	virtual SaveData getSaveData() override;
	virtual void fromSaveData(SaveData sd) override;
private:
	sf::Sprite resSprite;
	GameResource res;
};