#pragma once
#include "Pathfinder.h"
#include "Interactable.h"

namespace tg {


	class Quester : public Pathfinder, public Interactable {

	public:
		Quester(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
			int hitBoxW, int hitBoxH, int w, int h, int type, World* world, float speed);
		~Quester();

	protected:
		bool hasQuest = false;
		sf::Int32 timeAlive = 0;
		sf::Texture* dialogueTexture;
		std::vector<std::string> dialogueList;

	public:
		void tick(sf::Int32 dt) override;
		void render(Handler* handler) override;
		sf::Texture* getDialogueTexture() { return dialogueTexture; }
		std::string getDialogue() { srand(time(NULL)); return dialogueList[rand() % dialogueList.size()]; }


	};

}

