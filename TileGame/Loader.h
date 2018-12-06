#pragma once

#include <SFML/System.hpp>
#include "Handler.h"
#include <string>
namespace tg {
	class Loader
	{

	private:
		sf::Thread loadThread;
		Handler* handler;
		std::string loadingMessage = "";
		bool finished = false;

	public:
		Loader(Handler* handler);
		~Loader();

	public:
		void beginLoading();
		void exit();
		std::string getLoadingMessage() { return loadingMessage; }
		bool isFinished() { return finished; }

	private:
		void loadEntities();

	};

}