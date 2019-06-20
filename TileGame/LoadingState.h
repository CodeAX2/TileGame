#pragma once
#include "GameState.h"
#include "Handler.h"
#include "Loader.h"

namespace tg {

	class LoadingState :
		public GameState
	{

	private:
		Handler* handler;
		bool loaded = false;
		Loader loader;
		sf::Font loadingFont;
		float transp = 0;
		bool beganLoading = false;
		sf::Texture* controlsLayout;
		sf::Int32 timeOpen = 0;
		bool finished = false;
		sf::Int32 finishedFadeTransition = 0;


	public:
		LoadingState(Handler* handler);
		~LoadingState();


	public:
		void render();
		void tick(sf::Int32 dt);
		bool isLoaded() { return loaded; }
		void exit();

	};

}