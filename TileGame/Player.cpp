#include "Player.h"
#include "GameState.h"
#include "InputManager.h"
#include "Rideable.h"
#include "Building.h"
#include "PlayingState.h"
#include "DeathQuotes.h"

using namespace tg;

Player::Player(float x, float y, Handler* handler, World* world) : Entity(x, y, handler, 14, 117 - 25, 31, 25, 20 * 3, 39 * 3, true, PLAYER_E, true, world) {
	texture = handler->assets->getPlayerAnim()->getFrame(0);
	handler->player = this;

	health = 100;

}


Player::~Player()
{
}

// Render the player
void Player::render(Handler* handler) {

	int pX = (x - handler->camera->getXOffset()), pY = (y - handler->camera->getYOffset());

	int accuratePX = std::round(handler->window->getSize().x / 2.0 - 20.0*1.5);
	int accuratePY = std::round(handler->window->getSize().y / 2.0 - 39.0*1.5);

	if (std::abs(pX - accuratePX) <= 4) {
		pX = accuratePX;
	}

	if (std::abs(pY - accuratePY) <= 4) {
		pY = accuratePY;
	}

	sf::Sprite sprite(*texture);
	sprite.setPosition(pX, pY);


	float xScale = w / texture->getSize().x;
	float yScale = h / texture->getSize().y;


	sprite.setScale(xScale, yScale);

	sprite.setColor(handler->assets->getPlayerColor());


	if (horiDirection == WEST) {
		sprite.setScale(-xScale, yScale);
		sprite.move(w, 0);
	} else {
		sprite.setScale(xScale, yScale);
	}

	GameState* gs = handler->getCurrentState();

	if (gs->getType() == PLAYING) {
		PlayingState* ps = (PlayingState*)gs;

		if (pY >= (int)handler->window->getSize().y / 2 + 20) {
			ps->setGuiToBottom(false);
		} else {
			ps->setGuiToBottom(true);
		}
	}

	handler->window->draw(sprite);


	if (handler->inputManager->hitboxIsShown()) {

		sf::IntRect hitBox = getCollisionBox();

		sf::RectangleShape shape2(sf::Vector2f(hitBox.width, hitBox.height));
		shape2.setFillColor(sf::Color(255, 0, 0, 150));
		shape2.setPosition(hitBox.left - handler->camera->getXOffset(), hitBox.top - handler->camera->getYOffset());

		handler->window->draw(shape2);
		handler->window->draw(atBox);
	}




}

// Update the player
void Player::tick(sf::Int32 dt) {

	if (PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCurrentState())) {
		if (ps->getWorld() != world) {
			ps->setWorld(world);
		}
	}

	moving = false;

	bool* keys = handler->inputManager->getAllKeys();
	bool nowAttackKey = handler->inputManager->getAttackKey();

	// Check for attacking
	if (ridingOn == nullptr) {
		if (nowAttackKey && !previousAttackKey || attacking) {
			if (!attacking) {
				timeSinceAttackStart = 0;
			} else {
				timeSinceAttackStart += dt;
			}
			if (timeSinceAttackStart <= 300) {
				if (!attacking) hitEntities();
				attacking = true;
				if (horiDirection != STILL) {
					if (timeSinceAttackStart <= 150) { curAnim = 10; } else {
						curAnim = 11;
					}
				} else if (vertDirection == NORTH) {
					if (timeSinceAttackStart <= 150) { curAnim = 14; } else {
						curAnim = 15;
					}
				} else {
					if (timeSinceAttackStart <= 150) { curAnim = 12; } else {
						curAnim = 13;
					}
				}

				texture = handler->assets->getPlayerAnim()->getFrame(curAnim);

				return;
			} else {
				attacking = false;
				timeSinceAttackStart = 0;
				atBox = sf::RectangleShape(); // Set the attack box to nothing
				// since no longer attacking
			}
		}

		previousAttackKey = nowAttackKey;
	}


	if (handler->inputManager->getRunningKey() && stam > 0 &&
		(keys[0] || keys[1] || keys[2] || keys[3]) && !slowRegen) {
		speed = 0.15;
		stam -= dt / 20.f;
		if (stam <= 0) { stam = 0; slowRegen = true; }
		stamRegenCooldown = 0;
	} else {
		if (stamRegenCooldown >= 350) {
			if (slowRegen) {
				speed = 0.075;
				stam += dt / 80.f;
			} else {
				speed = 0.1;
				stam += dt / 40.f;
			}
			if (stam >= maxStam) { stam = maxStam; slowRegen = false; }
		}
		stamRegenCooldown += dt;
	}

	// Simple update of directions
	if (keys[0]) {
		vertDirection = NORTH;
	}

	if (keys[2]) {
		vertDirection = SOUTH;
	}

	if (keys[1]) {
		horiDirection = EAST;
	}

	if (keys[3]) {
		horiDirection = WEST;
	}

	if (keys[3] && keys[1]) {
		horiDirection = STILL;
	}


	if (keys[2] && keys[0]) {
		vertDirection = STILL;
	}

	if ((keys[0] || keys[2]) && horiDirection != STILL && !(keys[1] || keys[3])) {
		horiDirection = STILL;
	}

	if ((keys[1] || keys[3]) && vertDirection != STILL && !(keys[0] || keys[2])) {
		vertDirection = STILL;
	}


	// Update textures
	if (horiDirection == STILL && vertDirection == SOUTH) {

		if (curAnim < 6 || curAnim > 7) {
			curAnim = 6;
		}
		if (keys[2] && ridingOn == nullptr) {

			timeSinceLastAnim += dt;

			if (timeSinceLastAnim >= 150 / (speed * 10)) {

				timeSinceLastAnim = 0;
				curAnim++;
				if (curAnim >= 8) {
					curAnim = 6;
				}

			}


		} else {
			curAnim = 0;
			timeSinceLastAnim = 0;
		}


	} else if (horiDirection == STILL && vertDirection == NORTH) {
		if (curAnim < 8 || curAnim > 9) {
			curAnim = 8;
		}
		if (keys[0] && ridingOn == nullptr) {

			timeSinceLastAnim += dt;

			if (timeSinceLastAnim >= 150 / (speed * 10)) {

				timeSinceLastAnim = 0;
				curAnim++;
				if (curAnim >= 10) {
					curAnim = 8;
				}

			}


		} else {
			curAnim = 17;
			timeSinceLastAnim = 0;
		}
	} else if (horiDirection != STILL) {
		if (curAnim < 2 || curAnim > 5) {
			curAnim = 2;
		}
		if ((keys[1] || keys[3]) && ridingOn == nullptr) {

			timeSinceLastAnim += dt;

			if (timeSinceLastAnim >= 150 / (speed * 10)) {

				timeSinceLastAnim = 0;
				curAnim++;
				if (curAnim >= 6) {
					curAnim = 2;
				}

			}


		} else {
			curAnim = 16;
			timeSinceLastAnim = 0;
		}

	} else {
		curAnim = 0;
	}

	texture = handler->assets->getPlayerAnim()->getFrame(curAnim);

	// Actually move the player
	for (int i = 0; i < 4; i++) {
		if (ridingOn != nullptr) {
			float dx = 0, dy = 0;
			if (keys[0]) {
				dy += -1.0 / 2 * dt * speed;
			}

			if (keys[2]) {
				dy += 1.0 / 2 * dt * speed;
			}

			if (keys[1]) {
				dx += 1.0 / 2 * dt * speed;
			}

			if (keys[3]) {
				dx += -1.0 / 2 * dt * speed;
			}

			ridingOn->move(dx, dy);
		} else {
			float oldY = y;
			float oldX = x;
			if (keys[0]) {
				y -= 1.0 / 2 * dt * speed;
				moving = true;
			}

			if (keys[2]) {
				y += 1.0 / 2 * dt * speed;
				moving = true;
			}

			if (checkForCollision()) {
				y = oldY;
			}

			if (keys[1]) {
				x += 1.0 / 2 * dt * speed;
				moving = true;
			}

			if (keys[3]) {
				x -= 1.0 / 2 * dt * speed;
				moving = true;
			}

			if (checkForCollision()) {
				x = oldX;
			}
		}
	}

	for (Building* b : Building::getAllBuildings()) {
		if (b->positionIsEntrance((x + w / 2) / 96, (y + h + 20) / 96, world)) {
			if (PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCurrentState())) {
				World* wd = b->getWorld();
				ps->setWorld(wd);
				handler->player->setWorld(wd);
				handler->player->setPos(wd->getSpawn().x * 96 + 96 / 2 - w / 2, wd->getSpawn().y * 96);
				break;
			}
		} else if (b->positionIsExit((x + w / 2) / 96, (y + h / 3) / 96) && world == b->getWorld()) {
			if (PlayingState* ps = dynamic_cast<PlayingState*>(handler->getCurrentState())) {
				World* wd = b->getOutWorld();
				ps->setWorld(wd);
				handler->player->setWorld(wd);
				handler->player->setPos(b->getOutPos().x * 96 + 96 / 2 - w / 2, b->getOutPos().y * 96);
				break;
			}
		}

	}


	if (settingNewPos) {
		this->x = this->newX;
		this->y = this->newY;
		settingNewPos = false;
	}
	//handler->world->setTile((x + hitBoxX + hitBoxW/2) / 96, (y + hitBoxY + hitBoxH) / 96, 5);
}

// Check if the player collided with something solid
bool Player::checkForCollision() {
	sf::IntRect pBox = getCollisionBox();
	EntityManager* em = world->getEntityManager();

	int sX, sY, eX, eY;

	sX = pBox.left / 96;
	sY = pBox.top / 96;
	eX = (pBox.left + pBox.width) / 96;
	eY = (pBox.top + pBox.height) / 96;

	for (int y = sY; y <= eY; y++) {
		for (int x = sX; x <= eX; x++) {

			std::vector<Entity*> entitiesAtTile = em->getEntitiesAtTile(x, y);

			for (Entity* cur : entitiesAtTile) {

				if (cur == this) {
					continue;
				}

				sf::IntRect eBox = cur->getCollisionBox();

				if (cur->isRideable()) {
					if (Rideable* rCur = dynamic_cast<Rideable*>(cur)) {
						if (ridingOn == nullptr) {
							if (eBox.intersects(pBox)) {
								rCur->setRider(this);
								return true;
							}
						}

					}
				}

				if (eBox.intersects(pBox)) {
					return true;
				}

			}
		}
	}

	// Deal with rideables
	sf::IntRect extendedPBox = pBox;

	extendedPBox.top -= 9;
	extendedPBox.left -= 9;
	extendedPBox.width += 18;
	extendedPBox.height += 18;

	sX = extendedPBox.left / 96;
	sY = extendedPBox.top / 96;
	eX = (extendedPBox.left + extendedPBox.width) / 96;
	eY = (extendedPBox.top + extendedPBox.height) / 96;

	for (int y = sY; y <= eY; y++) {
		for (int x = sX; x <= eX; x++) {

			std::vector<Entity*> entitiesAtTile = em->getEntitiesAtTile(x, y);

			for (Entity* cur : entitiesAtTile) {

				if (cur == this) {
					continue;
				}

				if (cur->isRideable()) {
					if (Rideable* rCur = dynamic_cast<Rideable*>(cur)) {
						if (ridingOn == nullptr) {
							if (rCur->getCollisionBox().intersects(extendedPBox)) {
								rCur->setRider(this);
								return true;
							}
						}

					}
				}
			}
		}
	}

	int cX = x + hitBoxX, cY = y + hitBoxY;

	if (cX < 0 || cX + hitBoxW >= world->getWidth() * 96 ||
		cY < 0 || cY + hitBoxH >= world->getHeight() * 96) {
		return true;
	}


	if (world->tileIsSolid(pBox.left, pBox.top) ||
		world->tileIsSolid(pBox.left + pBox.width, pBox.top) ||
		world->tileIsSolid(pBox.left + pBox.width, pBox.top + pBox.height) ||
		world->tileIsSolid(pBox.left, pBox.top + pBox.height)) {
		return true;
	}

	if (Building::buildingOccupies(pBox.left / 96, pBox.top / 96, world) ||
		Building::buildingOccupies((pBox.left + pBox.width) / 96, pBox.top / 96, world) ||
		Building::buildingOccupies((pBox.left + pBox.width) / 96, (pBox.top + pBox.height) / 96, world) ||
		Building::buildingOccupies(pBox.left / 96, (pBox.top + pBox.height) / 96, world)) {
		return true;
	}



	return false;
}

// Damage all entities in the direction of attack
void Player::hitEntities() {

	int xOffset = 0;
	int yOffset = 0;

	switch (vertDirection) {
	case NORTH:
		yOffset = -15;
		break;
	case SOUTH:
		yOffset = 15;
		break;
	case STILL:
		yOffset = 0;
	}

	switch (horiDirection) {
	case WEST:
		xOffset = -15;
		break;
	case EAST:
		xOffset = 15;
		break;
	case STILL:
		xOffset = 0;
		break;
	}

	if (xOffset == 0 && yOffset == 0) {
		yOffset = 15;
	}

	sf::IntRect aBox;
	aBox.width = hitBoxW + abs(xOffset);
	aBox.height = hitBoxH + abs(yOffset);

	if (xOffset < 0) {
		aBox.left = hitBoxX + xOffset + x;
	} else {
		aBox.left = hitBoxX + x;
	}

	if (yOffset < 0) {
		aBox.top = hitBoxY + yOffset + y;
	} else {
		aBox.top = hitBoxY + y;
	}

	atBox = sf::RectangleShape(sf::Vector2f(aBox.width, aBox.height));
	atBox.setFillColor(sf::Color(0, 255, 255, 150));
	atBox.setPosition(aBox.left - handler->camera->getXOffset(), aBox.top - handler->camera->getYOffset());


	EntityManager* em = world->getEntityManager();
	for (int i = 0; i < em->numEntities(); i++) {

		Entity* cur = em->getEntity(i);

		if (cur == this) {
			continue;
		}

		sf::IntRect eBox = cur->getCollisionBox();
		if (eBox.intersects(aBox)) {
			if (inventory[2] == 0) {
				cur->damage(20, this);
			} else {
				cur->damage(30, this);
			}

		}





	}



}

// Add an item to the player's inventory
void Player::addItemToInv(int itemId) {
	inventory[itemId]++;
}

// Remove an item from the player's inventory
void Player::removeItemFromInv(int itemId) {
	if (inventory[itemId] >= 1) {
		inventory[itemId]--;
	}
}

// Check to see if the player's inventory contains an item
bool Player::inventoryContains(int itemId) {
	if (inventory[itemId] == 0) {
		return false;
	}

	return true;
}

void Player::damage(int dmg, Entity* damager) {
	health -= dmg;
	if (health <= 0) {
		if (handler->getCurrentState()->getType() == PLAYING) {
			std::string playingMessage = "";
			PlayingState* state = dynamic_cast<PlayingState*>(handler->getCurrentState());
			playingMessage = DeathQuotes::getRandomDeathQuote(damager->type);
			state->playerDeath(playingMessage);
		}
	}

}