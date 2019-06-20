#include "Handler.h"
#include "GameState.h"
#include "LoadingState.h"
#include "PlayingState.h"
#include "WorldManager.h"
#include "MainMenuState.h"
#include "InventoryState.h"
#include "ChestInventoryState.h"
#include "CraftingState.h"
#include "DialogueState.h"

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
	allStates.push_back(new InventoryState(this));
	allStates.push_back(new ChestInventoryState(this));
	allStates.push_back(new CraftingState(this));
	allStates.push_back(new DialogueState(this));
	this->setGameState(MAIN_MENU);

}

// Set the current gameState
void Handler::setGameState(int stateId) {

	if (currentState != nullptr)
		currentState->pause();

	previousState = currentState;

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
	case INVENTORY:
		currentState = allStates[INVENTORY];
		break;
	case CHEST_INVENTORY:
		currentState = allStates[CHEST_INVENTORY];
		break;
	case CRAFTING_INVENTORY:
		currentState = allStates[CRAFTING_INVENTORY];
		break;
	case DIALOGUE:
		currentState = allStates[DIALOGUE];
		break;

	}

	currentState->resume();

}
