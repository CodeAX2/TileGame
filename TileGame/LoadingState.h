#pragma once
#include "GameState.h"
#include "Handler.h"
#include "Loader.h"

namespace tg {

	class LoadingState :
		public GameState
	{

	private:
		Handler * handler;
		bool loaded = false;
		Loader loader;
		sf::Font loadingFont;
		float transp = 0;
		bool beganLoading = false;

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