#pragma once
#include "GameState.h"
#include "Handler.h"
#include "Loader.h"
class LoadingState :
	public GameState
{

private:
	Handler* handler;
	bool loaded = false;
	Loader loader;
	sf::Font* loadingFont = nullptr;
	float transp = 0;

public:
	LoadingState(Handler* handler);
	~LoadingState();


public:
	void render();
	void tick(sf::Int32 dt);
	bool isLoaded() { return loaded; }
	void exit();

};

