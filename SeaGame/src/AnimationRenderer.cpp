#include "AnimationRenderer.h"

AnimationRenderer::AnimationRenderer(
	std::vector<sf::Sprite> sprites,
	float dur,
	size_t z,
	bool loop)
{
	this->sprites = sprites;
	this->duration = dur;
	this->index = 0;
	this->sClock.restart();
	this->zIndex = z;
	this->loop = loop;
}
void AnimationRenderer::render(RenderManager& r)
{
	// Increment index if the duration has passed
	if (this->sClock.getElapsedTime().asMilliseconds() > this->duration) {
		this->index++;
		// Check if it has ran through every sprite
		if (this->index >= this->sprites.size()) {
			// If looping, go back to first sprite
			if (this->loop) {
				this->index %= this->sprites.size();
			}
			else {
				// Remove parent
				this->getParent().lock()->game->removeEntity(this->getParent());
				return;
			}
		}
		this->sClock.restart();
	}
	sf::Sprite s = this->sprites[this->index];
	s.setPosition(this->getParent().lock()->components.transform->getPosition());
	r.add(s, this->zIndex);
}