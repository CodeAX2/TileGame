#include "GameState.h"

using namespace tg;

GameState::GameState(){}

GameState::GameState(int type) : type(type) {



}


GameState::~GameState() {
}


void GameState::render() {


}

void GameState::tick(sf::Int32 dt) {

}

void GameState::exit() {

}

// This is for when the game state is simply switched, and it is assumed
// that the current state will be used again
void GameState::pause() {

}

// Used whenever the state is switched to this one
void GameState::resume() {

}

// Not much here, as this is just the base of other classes