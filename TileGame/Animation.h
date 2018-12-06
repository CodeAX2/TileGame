#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
class Animation
{

private:
	std::vector<sf::Texture*> animFrames;
	int currentFrame = 0;

public:
	Animation();
	~Animation();

public:
	void addFrame(sf::Texture* t);
	sf::Texture* getFrame(int index);
	sf::Texture* getCurrentFrame();
	void nextFrame();
	int getSize() { return animFrames.size(); }


};

