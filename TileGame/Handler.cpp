#include "Handler.h"
#include "GameState.h"
#include "LoadingState.h"
#include "PlayingState.h"
#include "WorldManager.h"
#include "MainMenuState.h"

using namespace tg;

Handler::Handler() {

}

// Delete all of the things!
Handler::~Handler() {

	delete mainWorld;
	delete assets;
	delete camera;

	if (entityManager != nullptr) {
		entityManager->deleteEntities();
	}

	

	delete entityManager;
	delete inputManager;
	for (int i = 0; i < allStates.size(); i++) {
		delete allStates[i];
	}


	delete window;

}

// Create playing and loading states
void Handler::initGameStates() {
	allStates.push_back(new PlayingState(this));
	allStates.push_back(new LoadingState(this));
	allStates.push_back(new MainMenuState(this));

}

// Set the current gameState
void Handler::setGameState(int stateId) {


	switch (stateId) {
	case LOADING:
		currentState = allStates[LOADING];
		break;
	case PLAYING:
		currentState = allStates[PLAYING];
		break;
	case MAIN_MENU:
		currentState = allStates[MAIN_MENU];
		break;
	}

}
