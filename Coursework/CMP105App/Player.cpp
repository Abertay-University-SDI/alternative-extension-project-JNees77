#include "Player.h"
#include <iostream>

Player::Player()
{
	//sets the position and size
	setPosition(sf::Vector2f( 100, 100));
	setSize(sf::Vector2f(100, 100));

	//loads the texture file
	dino.loadFromFile("gfx/dinoVita.png");
	
	setTexture(&dino); // set that texture to this object.

	// dino texture is 24x24px. Frames for walk are 4 - 9, inclusive.
	for (int i = 4; i < 10; ++i)
	{
		walk.addFrame(sf::IntRect(i*24.f, 0.f, 24.f, 24.f));
	}
	for (int i = 0; i < 4; ++i)
	{
		kick.addFrame(sf::IntRect(i * 24.f + 240.f, 0.f, 24.f, 24.f));
	}
	damaged.addFrame(sf::IntRect(0, 0, 24, 24));
	damaged.addFrame(sf::IntRect(15*24, 0, 24, 24));
	damaged.setFrameSpeed(1.f / 4.f);
	currentAnimation = &walk;
	walk.setFrameSpeed(1.f / 10.f);
	//kick.setLooping(false);

	velocity = { 0,0 };
}

Player::~Player()
{
}
//method to initialise the values for jumping
void Player::setJumping(float jumpH, float jumpLength)
{
	jumpTime = jumpLength;
	jumpTimeElapsed = 0;
	jumpHeight = jumpH;
}

bool Player::isKicking()
{
	return kickTimeElapsed > 0 && kickTimeElapsed < kickTime;
}

void Player::setKicking(float t)
{
	//sets the animation to the kick animation and sets the time of the kick
	kick.setFrameSpeed(t / kick.getSize());
	kick.reset();
	currentAnimation = &kick;
	kickTime = t;
	kickTimeElapsed = 0.f;
}
//method to check if the player can jump again
bool Player::canJump() const
{
	return jumpTime == 0;
}
//flips the current animation
void Player::setFlipped(bool f)
{
	currentAnimation->setFlipped(f);
}

void Player::handleInput(float dt)
{

}
//returns if the enemy hits an obstacle
bool Player::isDamaged()
{
	return currentAnimation == &damaged;
}
//plays the animation for the player being damaged
void Player::setDamaged(float timer)
{
	currentAnimation = &damaged;
	damageLength = timer;
}
//update method
void Player::update(float dt)
{
	//updates the time the damaged animation has been playing
	if (currentAnimation == &damaged)
		damagedTimer += dt;
	//after a specific time sets the animation back to the walk animation
	if (damagedTimer > damageLength)
	{
		currentAnimation = &walk;
		damagedTimer = 0.f;
	}

	// check for jump
	if (jumpTimeElapsed < jumpTime)
	{

		jumpTimeElapsed += dt;
		//jump equation
		// go up.
		if (jumpTimeElapsed < jumpTime * 0.2)
		{
			move(0, -jumpHeight * dt / (jumpTime * 0.2));
		}
		// hover for a bit
		// go down.
		if (jumpTimeElapsed > jumpTime * 0.7)
		{
			move(0, jumpHeight * dt / (jumpTime * 0.3));
		}

	}
	else
	{
		//resets the jump values (so the character doesnt go under the screen
		jumpHeight = 0;
		jumpTimeElapsed = 0.f;
		jumpTime = 0.f;
	}

	// check for kick status.
	if (kickTime != 0)
	{
		//increases the kick time counter
		if (kickTimeElapsed < kickTime) kickTimeElapsed += dt;
		else
		{
			//sets the animation to walk and resets the kick variables
			currentAnimation = &walk;
			kickTime = 0.f;
			kickTimeElapsed = 0.f;
		}
	}
	//animates the current animation
	currentAnimation->animate(dt);
	//sets the texture to the next frame of the curent animation
	setTextureRect(currentAnimation->getCurrentFrame());

}