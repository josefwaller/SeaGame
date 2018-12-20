#pragma once
#include "BaseRenderer.h"

class MilitaryBaseRenderer : public BaseRenderer
{
public:
	MilitaryBaseRenderer();
	virtual void render(RenderManager& r) override;
};