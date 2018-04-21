#pragma once
#include "BaseRenderer.h"

class MilitaryBaseRenderer : public BaseRenderer
{
public:
	MilitaryBaseRenderer(std::weak_ptr<Entity> parent);
	virtual void render(RenderManager& r) override;
};