#include "BuildingEntity.h"
#include "PlayingState.h"


using namespace tg;

BuildingEntity::BuildingEntity(float x, float y, Handler* handler, int hitBoxX, int hitBoxY,
	int hitBoxW, int hitBoxH, int w, int h, bool needsTicking, int type,
	bool needsInitialization, World* world) :

	Entity(
		x, y, handler, hitBoxX, hitBoxY, hitBoxW,
		hitBoxH, w, h, needsTicking, type, needsInitialization, world
	) {

	lightX = x - 1;
	lightY = y - 1;


}


BuildingEntity::~BuildingEntity()
{
}

void BuildingEntity::renderLighting(Handler* handler) {
	PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCustomState(PLAYING));
	if (ps->getDarknessPercent() != 0) {
		sf::Color color(255, 255, 255, lightIntensity);

		lightSizeX = texture->getSize().x * 3 + 2;
		lightSizeY = texture->getSize().y * 3 + 2;

		sf::RectangleShape shadow(sf::Vector2f(lightSizeX, lightSizeY));
		shadow.setPosition(
			(int)(lightX - floor(handler->camera->getXOffset())),
			(int)(lightY - floor(handler->camera->getYOffset())));

		shadow.setTexture(texture);

		sf::BlendMode bm2(
			sf::BlendMode::Factor::Zero,
			sf::BlendMode::Factor::DstColor,
			sf::BlendMode::Equation::Add,
			sf::BlendMode::Factor::One,
			sf::BlendMode::Factor::One,
			sf::BlendMode::Equation::ReverseSubtract
		);

		ps->getLightRenderer()->draw(shadow, bm2);


		sf::BlendMode bm(
			sf::BlendMode::Factor::Zero,
			sf::BlendMode::Factor::DstColor,
			sf::BlendMode::Equation::Add,
			sf::BlendMode::Factor::One,
			sf::BlendMode::Factor::One,
			sf::BlendMode::Equation::Add
		);

		ps->getLightRenderer()->draw(shadow, bm);
	}
}
