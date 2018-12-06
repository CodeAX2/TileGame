#pragma once
#include "GameState.h"
#include "Handler.h"
class BattleState :
	public GameState
{
public:
	BattleState(Handler* handler);
	~BattleState();

private:
	Handler* handler;
	sf::Int32 timeSinceStart = 0;
	float transitionValue = 0;
	bool transitionFinished = false;
	std::vector<sf::RectangleShape> transitionSquares;


public:
	void render();
	void tick(sf::Int32 dt);

};

