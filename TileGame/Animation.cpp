#include "Animation.h"



Animation::Animation() {



}


Animation::~Animation() {
	for (sf::Texture* t : animFrames) {
		delete t;
	}
}

void Animation::addFrame(sf::Texture* t) {
	animFrames.push_back(t);
}

sf::Texture* Animation::getFrame(int index) {
	if (index < animFrames.size()) {
		return animFrames[index];
	}
	return nullptr;
}

sf::Texture* Animation::getCurrentFrame() {
	if (animFrames.size() != 0) {
		return animFrames[currentFrame];
	}
	return nullptr;
}

void Animation::nextFrame() {
	currentFrame++;
	if (currentFrame >= animFrames.size()) {
		currentFrame = 0;
	}
}
