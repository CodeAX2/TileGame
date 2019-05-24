#include "DialogueState.h"
#include "PlayingState.h"
#include "InputManager.h"
#include "Assets.h"

using namespace tg;

DialogueState::DialogueState(Handler* handler) : GameState(DIALOGUE) {
	this->handler = handler;

	dialogueBg = handler->assets->loadTextureFromResource(MENU_DIALOGUE);

}


DialogueState::~DialogueState() {
}

void DialogueState::render() {

	if (guiFont.getInfo().family == "") {
		guiFont = handler->assets->getArialiFont();
	}

	handler->getCustomState(PLAYING)->render();

	renderDialogue();

}

void DialogueState::tick(sf::Int32 dt) {
	timeOpen += dt;
	if (handler->inputManager->dialogueKey && !prevDialogueKey) {
		// Pressed E
		if (!fullText) {
			fullText = true;
		} else {
			handler->setGameState(PLAYING);
		}
	}

	prevDialogueKey = handler->inputManager->dialogueKey;

	if (dialogueString == "" && talkingQuester != nullptr) {
		dialogueString = talkingQuester->getDialogue();
	}
}

void DialogueState::resume() {
	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	if (!ps->musicIsPaused())
		ps->playBGMusic();

	timeOpen = 0;
	fullText = false;
	prevDialogueKey = true;

}

void DialogueState::pause() {
	handler->inputManager->disableCurrentMovement();
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	if (ps->musicIsPaused())
		ps->pauseBGMusic();

	dialogueString = "";

}

void DialogueState::renderDialogue() {

	sf::RectangleShape bg(handler->window->getView().getSize());
	bg.setTexture(dialogueBg);

	handler->window->draw(bg);

	sf::RectangleShape npc(sf::Vector2f(64 * 3, 64 * 3));
	npc.setTexture(talkingQuester->getDialogueTexture());
	npc.setPosition(39, 489);

	handler->window->draw(npc);

	std::vector<std::string> dialogueWords;

	// Split the string into words
	std::string curWord = "";
	for (int i = 0; i < dialogueString.size(); i++) {
		if (dialogueString.at(i) != ' ') {
			curWord += dialogueString.at(i);
		} else {
			dialogueWords.push_back(curWord);
			curWord.clear();
		}
	}
	if (curWord != "") dialogueWords.push_back(curWord);

	int totalChars = timeOpen / 50;
	if (fullText) totalChars = INT_MAX;

	int row = 0;
	int rowTotal = 0;
	int charsPrinted = 0;
	for (int i = 0; i < dialogueWords.size(); i++) {
		std::string s = dialogueWords[i];
		bool finalWord = false;

		sf::Text dialogueText(s, guiFont);
		dialogueText.setCharacterSize(20);

		if (rowTotal + dialogueText.getGlobalBounds().width > 898) {
			rowTotal = 0;
			row++;
		}

		if (s.size() + charsPrinted > totalChars) {
			s = s.substr(0, totalChars - charsPrinted);
			finalWord = true;
			dialogueText.setString(s);
		}

		if (!finalWord) {
			dialogueText.setString(dialogueText.getString() + " ");
			charsPrinted += s.length();
		}
		dialogueText.setPosition(266 + 10 + rowTotal, 489 + 12 + row * (guiFont.getGlyph('!', 20, false).bounds.height + 6));
		dialogueText.setFillColor(sf::Color::White);

		rowTotal += dialogueText.getGlobalBounds().width;

		handler->window->draw(dialogueText);

		if (finalWord || charsPrinted == totalChars) return;
	}

	fullText = true;


}