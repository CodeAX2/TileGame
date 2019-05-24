#pragma once
#include "GameState.h"
#include "Handler.h"
#include "Quester.h"

namespace tg {

	class DialogueState :
		public GameState {
	public:
		DialogueState(Handler* handler);
		~DialogueState();

	private:
		Handler* handler;
		Quester* talkingQuester;
		std::string dialogueString;
		sf::Texture* dialogueBg;
		sf::Int32 timeOpen;
		bool prevDialogueKey = false;
		bool fullText = false;


	public:
		void render() override;
		void tick(sf::Int32 dt) override;
		void resume() override;
		void pause() override;
		//void updateMouse() override;
		//void mouseClicked(sf::Event e) override;
		//void updateJoystick(sf::Int32 dt) override;

		void setTalkingQuester(Quester* quester) { talkingQuester = quester; }

	private:
		void renderDialogue();

	};

}