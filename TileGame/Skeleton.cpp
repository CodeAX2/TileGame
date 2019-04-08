#include "Skeleton.h"
#include "Item.h"
#include "Player.h"
#include "InputManager.h"

using namespace tg;

const float Skeleton::SPEED = .15f;

Skeleton::Skeleton(float x, float y, Handler* handler, World* world) :
	Pathfinder(x, y, handler, 16 * 2, 32 * 3 - 25, 32, 25, 32 * 3, 32 * 3, SKELETON_E, world, SPEED) {

	texture = handler->assets->getSkeletonAnimation()->getFrame(curAnim);
	this->health = 60;
	this->maxHealth = 60;

}


Skeleton::~Skeleton()
{
}


void Skeleton::dropItems() {
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);

	if (rand() % 2 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	}

	if (rand() % 2 == 0) {
		new Item(x + (float)w / 2 - 32 + rand() % 21 - 10, y + h - 64 + rand() % 21 - 10, handler, 1, world);
	}


}

void Skeleton::onCollisionWithFollowing(sf::Int32 dt) {
	if (following == handler->player) {
		if (!attacking) {
			attacking = true;
			following->damage(10, this);
		}

	}
}

void Skeleton::render(Handler* handler) {

	texture = handler->assets->getSkeletonAnimation()->getFrame(curAnim);

	// Draw the entity itself
	sf::Sprite sprite(*texture);
	sprite.setPosition(x - handler->camera->getXOffset(), y - handler->camera->getYOffset());

	float xScale = w / texture->getSize().x;
	float yScale = h / texture->getSize().y;


	sprite.setScale(xScale, yScale);


	if (facing == WEST) {
		sprite.setScale(-xScale, yScale);
		sprite.move(w, 0);
	} else {
		sprite.setScale(xScale, yScale);
	}

	handler->window->draw(sprite);

	// Draw the health bar, if health is in [0, maxHealth)
	// The health bar changes color based on the health variable
	if (health < maxHealth && health >= 0) {
		sf::Text healthText;
		healthText.setFont(handler->assets->getArialiFont());

		std::stringstream ss;
		ss << health << "/" << maxHealth;

		healthText.setString(ss.str());
		healthText.setCharacterSize(15);
		healthText.setPosition((int)(x - floor(handler->camera->getXOffset())) - healthText.getLocalBounds().width / 2 + w / 2, (int)(y - 27 - floor(handler->camera->getYOffset())));
		if (health > maxHealth || health < 0 || handler == nullptr) return;

		sf::RectangleShape textBg(sf::Vector2f(w, healthText.getGlobalBounds().height + 4));
		textBg.setPosition(sf::Vector2f(x - floor(handler->camera->getXOffset()), healthText.getGlobalBounds().top - 2));
		textBg.setFillColor(sf::Color(0, 0, 0, 140));
		handler->window->draw(textBg);

		handler->window->draw(healthText);


		sf::RectangleShape healthBar(sf::Vector2f(w * (float)health / maxHealth, 10));
		healthBar.setPosition((int)(x - floor(handler->camera->getXOffset())), (int)(y - 10 - floor(handler->camera->getYOffset())));
		if (health > maxHealth / 2.f) {
			healthBar.setFillColor(sf::Color(
				255 - (float)health / (maxHealth / 2.f) * 255,
				255,
				(float)health / maxHealth * 50,
				180
			));
		} else {
			healthBar.setFillColor(sf::Color(
				255,
				(float)health / (maxHealth * .6f) * 255,
				(float)health / maxHealth * 50,
				180
			));
		}
		handler->window->draw(healthBar);
	}


	// Draw the hitbox if hitboxes are being shown
	if (handler->inputManager->hitboxIsShown()) {
		sf::IntRect hitBox = getCollisionBox();

		sf::RectangleShape shape2(sf::Vector2f(hitBox.width, hitBox.height));
		shape2.setFillColor(sf::Color(255, 0, 0, 150));
		shape2.setPosition(hitBox.left - handler->camera->getXOffset(), hitBox.top - handler->camera->getYOffset());

		handler->window->draw(shape2);
	}


	if (handler->inputManager->hitboxIsShown()) {
		std::vector<sf::Vector2i> cP = currentPath;
		for (sf::Vector2i pos : cP) {
			sf::RectangleShape s;
			s.setPosition(sf::Vector2f(pos.x * pathfindSize - handler->camera->getXOffset(), pos.y * pathfindSize - handler->camera->getYOffset()));
			s.setSize(sf::Vector2f(pathfindSize, pathfindSize));
			s.setFillColor(sf::Color(0, 0, 255, 175));
			handler->window->draw(s);
		}

		if (cP.size() != 0) {
			float tX = cP[spotInpath].x * pathfindSize - handler->camera->getXOffset();
			float tY = cP[spotInpath].y * pathfindSize - handler->camera->getYOffset();
			float cX = x + hitBoxX - handler->camera->getXOffset();
			float cY = y + hitBoxY - handler->camera->getYOffset();

			sf::VertexArray line(sf::LinesStrip, 2);
			line[0].position = sf::Vector2f(cX, cY);
			line[1].position = sf::Vector2f(tX, tY);

			line[0].color = sf::Color::Red;
			line[1].color = sf::Color::Red;


			handler->window->draw(line);
		}


	}
}


void Skeleton::tick(sf::Int32 dt) {

	if (following == nullptr && followingId != GUID_NULL) {
		following = world->getEntityManager()->getEntityById(followingId);
	}

	bool moved = false;

	// Make sure destination is correct
	if (following != nullptr) {
		if (pow(following->getX() - x, 2) + pow(following->getY() - y, 2) >= 960 * 960) {
			if (pow(following->getX() - x, 2) + pow(following->getY() - y, 2) >= 6144 * 6144) {
				world->getEntityManager()->removeEntity(this, true);
				return;
			}
			active = false;
		} else {
			active = true;
		}
	}

	if (beingKnockbacked) {
		knockbackTime -= dt;
		if (knockbackTime < 0) {
			beingKnockbacked = false;

		}

		float ogX = x;
		float ogY = y;

		x += knockbackDistance * cos(knockbackDegree) / (float)timeToKnockback * dt * (1.3f *(float)knockbackTime / (float)timeToKnockback);


		if (checkForCollision(x, y)) {
			x = ogX;
		}

		y += knockbackDistance * sin(knockbackDegree) / (float)timeToKnockback * dt * (1.3f * (float)knockbackTime / (float)timeToKnockback);
		if (checkForCollision(x, y)) {
			y = ogY;
		}

		roundedHitBox = sf::IntRect(sf::Vector2i(std::round(x + (float)hitBoxX), std::round(y + (float)hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		world->getEntityManager()->fixEntityMoved(this, ogX, ogY);


	}

	if (attacking) {
		attackCooldown += dt;
		if (attackCooldown >= 500) {
			attacking = false;
			attackCooldown = 0;
		} else if (attackCooldown >= 250) {
			switch (facing) {
			case NORTH:
				curAnim = 20;
				break;
			case SOUTH:
				curAnim = 4;
				break;
			case EAST:
				curAnim = 14;
				break;
			case WEST:
				curAnim = 14;
				break;

			}
		} else {
			switch (facing) {
			case NORTH:
				curAnim = 19;
				break;
			case SOUTH:
				curAnim = 3;
				break;
			case EAST:
				curAnim = 13;
				break;
			case WEST:
				curAnim = 13;
				break;

			}
		}

		return;
	}

	if (!active) {
		// If we are too far from following, don't move. Fix textures

		if (curAnim >= 1 && curAnim <= 2) {
			curAnim = 0;
		} else if (curAnim >= 9 && curAnim <= 12) {
			curAnim = 8;
		} else if (curAnim >= 17 && curAnim <= 18) {
			curAnim = 16;
		}

		directionX = STILL; directionY = STILL;

		return;
	}

	std::vector<sf::Vector2i> cP = currentPath;

	if (cP.size() != 0 && spotInpath < cP.size()) {

		float dx = cP[spotInpath].x * pathfindSize - (x + hitBoxX);
		float dy = cP[spotInpath].y * pathfindSize - (y + hitBoxY);

		float angle = atan2(dy, dx);



		float dist = speed * dt;

		float ogX = x;
		float ogY = y;

		bool moved = true;


		if (checkForCollision(x, y)) {
			// Collision before we moved, so disable collision for a sec
			x += dist * cos(angle);
			y += dist * sin(angle);
		} else {

			x += dist * cos(angle);

			if (checkForCollision(x, y)) {
				x = ogX;
			}

			y += dist * sin(angle);
			if (checkForCollision(x, y)) {
				y = ogY;
			}

			if (x == ogX && y == ogY) {
				moved = false;
			}

		}

		if (checkForCollisionWithFollowing(x, y)) {
			onCollisionWithFollowing(dt);
		}

		float checkX = cP[spotInpath].x * pathfindSize;
		float checkY = cP[spotInpath].y * pathfindSize;

		if (linesCross(ogX, ogY, x, y, checkX, checkY, checkX + pathfindSize, checkY)) {
			spotInpath++;
			x = checkX - hitBoxX; y = checkY - hitBoxY;
			dx = 0; dy = 0;
		} else if (linesCross(ogX, ogY, x, y, checkX, checkY, checkX, checkY + pathfindSize)) {
			spotInpath++;
			x = checkX - hitBoxX; y = checkY - hitBoxY;
			dx = 0; dy = 0;
		} else if (abs(dx) <= .25f && abs(dy) <= .25f) {
			spotInpath++;
			x = checkX - hitBoxX; y = checkY - hitBoxY;
			dx = 0; dy = 0;
		} else if (spotInpath == 0 && !moved) {
			spotInpath++;
		}

		if (spotInpath >= cP.size()) {
			spotInpath = 0;
		}

		if (pathIsQueued) {
			while (adjustingPath);
			adjustingPath = true;
			currentPath = queuedPath;
			pathIsQueued = false;
			spotInpath = 0;
			adjustingPath = false;
		}

		roundedHitBox = sf::IntRect(sf::Vector2i(std::round(x + (float)hitBoxX), std::round(y + (float)hitBoxY)), sf::Vector2i(hitBoxW, hitBoxH));
		world->getEntityManager()->fixEntityMoved(this, ogX, ogY);
		timeSinceLastAnim += dt;

		int newAnim = curAnim;

		if ((directionX != STILL || directionY != STILL)) {
			if (timeSinceLastAnim >= 150 / (speed * 10)) {
				timeSinceLastAnim = 0;
				newAnim++;
				timeSinceDirChange = 11; // We need to update textures
			}
		}

		timeSinceDirChange += dt;
		if (timeSinceDirChange > 10) {
			if (dy > 0) {
				directionY = SOUTH;
				facing = SOUTH;
				if ((newAnim > 2 || newAnim < 1) && directionX == STILL)
					newAnim = 1;
			} else if (dy < 0) {
				directionY = NORTH;
				facing = NORTH;
				if ((newAnim < 17 || newAnim > 18) && directionX == STILL)
					newAnim = 17;
			}

			if (y == ogY) {
				if (directionY == NORTH && directionX == STILL) {
					newAnim = 17;
				} else if (directionY == SOUTH && directionX == STILL) {
					newAnim = 1;
				}
				directionY = STILL;
			}

			if (dx > 0) {
				directionX = EAST;
				facing = EAST;
				if (newAnim < 9 || newAnim > 12)
					newAnim = 9;
			} else if (dx < 0) {
				if (newAnim < 9 || newAnim > 12)
					newAnim = 9;
				directionX = WEST;
				facing = WEST;
			}

			if (x == ogX) {
				if (directionX == EAST || directionX == WEST) {
					newAnim = 9;
				}
				directionX = STILL;
			}

			timeSinceDirChange = 0;
		}

		curAnim = newAnim;

	} else {

		directionX = STILL;
		directionY = STILL;

		if (pathIsQueued) {
			while (adjustingPath);
			adjustingPath = true;
			currentPath = queuedPath;
			spotInpath = 0;
			pathIsQueued = false;
			adjustingPath = false;
		}
	}

	if (directionX == STILL && directionY == STILL) {
		if (curAnim >= 1 && curAnim <= 2) {
			curAnim = 0;
		} else if (curAnim >= 9 && curAnim <= 12) {
			curAnim = 8;
		} else if (curAnim >= 17 && curAnim <= 18) {
			curAnim = 16;
		}
	}


}

void Skeleton::damage(int dmg, Entity* damager) {

	if (damager != nullptr) {
		following = damager;
		followingId = damager->getId();

		beingKnockbacked = true;
		knockbackTime = timeToKnockback;

		knockbackDegree = atan2(y + hitBoxY + hitBoxH / 2 - damager->getCollisionBox().top - damager->getCollisionBox().height / 2, x + hitBoxX + hitBoxW / 2 - damager->getCollisionBox().left - damager->getCollisionBox().width / 2);
		knockbackDegree = round(knockbackDegree / (3.1415 / 2));
		knockbackDegree *= (3.1415 / 2);


	}

	Entity::damage(dmg, damager);
}
