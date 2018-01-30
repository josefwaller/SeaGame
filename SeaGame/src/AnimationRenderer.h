#pragma once
#include "RenderComponent.h"

class AnimationRenderer : public RenderComponent
{
public:
	AnimationRenderer(std::weak_ptr<Entity> parent, std::vector<sf::Sprite> sprites, float dur, size_t z, bool loop);
	virtual void render(RenderManager& r) override;
private:
	// The sprites to flip through
	std::vector<sf::Sprite> sprites;
	// The duration between flipping sprites
	float duration;
	// Index of the sprite currently being shown
	size_t index;
	// Count time between sprites
	sf::Clock sClock;
	// The z index to draw the sprite
	size_t zIndex;
	// Whether to loop the entity or to detroy the entity when the animation finished
	bool loop;
};
