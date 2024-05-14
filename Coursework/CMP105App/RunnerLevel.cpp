#include "RunnerLevel.h"

//gets random minimum and maximum numbers
int getRandomInt(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(gen);
}
//constructor
RunnerLevel::RunnerLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	textMan = tm;

	//allows for a change in the level everytime the code runs
	std::srand(static_cast<unsigned>(std::time(nullptr)));

	// setup BGs as ten images next to each other. base dimensions 1024x1024
	float bgScalar = hwnd->getSize().y / 1024.0f;
	//creates one big image from one smaller square image, using i to repeat it
	for (int i = 0; i < 21; ++i)
	{
		GameObject bg;
		bg.setTexture(&textMan->getTexture("bg_Scroll"));
		bg.setSize(sf::Vector2f(1024.0f*bgScalar , 1024.0f*bgScalar));
		bg.setPosition(i * 1024 * bgScalar, 0);
		BGs.push_back(bg);
	}

	distance = 19 * BGs.back().getSize().x - 30;
	//finish line is just a yellow rectangle
	finishLine.setSize(sf::Vector2f(10, window->getSize().y * 0.2));
	//set at the end of the level
	finishLine.setPosition(distance, window->getSize().y * 0.5);
	finishLine.setFillColor(sf::Color::Yellow);

	float placementIndex = 0.f;
	while (placementIndex < distance)
	{
		// go forward a random distance:
		placementIndex += getRandomInt(650,800);
		// harder in the back half.
		if (placementIndex > distance / 2)
			placementIndex -= getRandomInt(100, 250);	
		if (placementIndex > distance) 
			break;
		//increase the number of objects
		objects += 1;
		//creates a new game object
		GameObject newObj;
		//sets the position of the object
		newObj.setPosition(placementIndex, window->getSize().y * 0.6);
		//sets its size
		newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
		// randomly put a jumpable, kickable or two-jumpables next to each other
		int seed = getRandomInt(0, 2);
		switch (seed)
		{
		case 0:
			// kickable
			newObj.setTexture(&textMan->getTexture("kickable"));
			newObj.setPosition(placementIndex, window->getSize().y * 0.5);
			newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.2));
			kickables.push_back(newObj);
			break;
		case 1:
			// jumpable
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			break;
		case 2:
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			//if two jumpable boxes stack on each other this code executes
			GameObject secondObj = newObj;
			secondObj.setPosition(secondObj.getPosition().x + secondObj.getSize().x, secondObj.getPosition().y);
			jumpables.push_back(secondObj);
			break;
		}
	}
	
	// setup Player
	p.setPosition(30, window->getSize().y * 0.6);
	speed = 0.f;
	//this is a smaller version of the character represented by a white square
	progressP.setSize(sf::Vector2f(window->getSize().y * 0.04, window->getSize().y * 0.04));
	progressP.setPosition(0.25 * window->getSize().x, 0.03 * window->getSize().y);
	progressLine.setSize(sf::Vector2f(window->getSize().x * 0.5, window->getSize().y * 0.01));
	progressLine.setFillColor(sf::Color::White);
	progressLine.setPosition(0.25 * window->getSize().x, 0.05 * window->getSize().y);
	//this is a circle object to show how far the player is through the level
	destinationPoint.setRadius(10);
	destinationPoint.setPosition(0.75 * window->getSize().x + 5, 0.05 * window->getSize().y - 5);
	destinationPoint.setFillColor(sf::Color::Green);
	//this is the lecturers head in the top right corner of the window
	moon.setPosition(0.9 * window->getSize().x, 0);
	moon.setSize(sf::Vector2f(0.1 * window->getSize().x, 0.1 * window->getSize().x));
	moon.setTexture(&textMan->getTexture("lecturer"));
	moon.setTextureRect(sf::IntRect(0, 0, 24, 24));
	//if the player runs into a box this executes
	p.setDamaged(0.5);

}

RunnerLevel::~RunnerLevel()
{
}

void RunnerLevel::handleInput(float dt)
{
	//if space is pressed uses the jump code in player to jump
	if (input->isKeyDown(sf::Keyboard::Space) && p.canJump())
	{
		p.setJumping(window->getSize().y*0.25, 0.85);
	}
	//uses the kick method in player
	if (input->isKeyDown(sf::Keyboard::Enter) && !p.isKicking())
	{
		p.setKicking(0.5);
	}
	if (input->isPressed(sf::Keyboard::P))
	{
		//stores the game state to be used by the pause state later
		gameState->storePreviousState(gameState->getCurrentState());
		gameState->setCurrentState(State::PAUSE);
	}
}

bool RunnerLevel::colliding(GameObject obj)
{
	//checks if the bounds of an object come into contact with a player
	if (obj.getGlobalBounds().intersects(p.getGlobalBounds()))
	{
		//if yes then true
		return true;
	}

	return false;
}

void RunnerLevel::update(float dt)
{
	// race over;
	if (travelled >= distance)
	{
		//changes the game state to the game over screen for this game and shows the 
		//number of deaths and time of the run as well as the number of obstacles the player hit
		gameState->addResult("l2deaths", hits);
		gameState->addResult("l2time", time);
		if (gameState->getSingleRun())
		{
			gameState->setCurrentState(State::ENDGAME);
		}
		else
		{
			gameState->setCurrentState(State::PRE_THREE);
		}
		return;
	}
	//adds 1 to time for every seconds that passes
	time += dt;
	//calls the player update method
	p.update(dt);
	progressP.update(dt);
	//calculates the distance the player has travelled respective to the size of the window
	float progress = (travelled / distance) * window->getSize().x * 0.5;
	progressP.setPosition(0.25 * window->getSize().x + progress, 0.03 * window->getSize().y);
	//calculation to find the distance travlled
	travelled += dt * speed;

	// scroll stuff
	for (GameObject& bg : BGs)
	{
		//moves the obstacles along the x axis
		bg.move(-dt * speed, 0);
	}
	for (GameObject& j : jumpables)
	{
		//moves the jumpable objects towarss the player
		j.move(-dt * speed, 0);
	}
	for (GameObject& k : kickables)
	{
		//moves the kickables towards the player
		k.move(-dt * speed, 0);
	}
	for (GameObject& e : explosions)
	{
		//moves the explosions towards the player
		e.move(-dt * speed, 0);
	}
	//finally moves the finish line towards the player
	finishLine.move(- dt * speed, 0);

	// speed increases to max. Changes animation speed.
	if (speed < MAX_SPEED)
		//applies acceleration to increase the speed over time
		speed += dt * ACCELERATION;
	if (!p.isKicking())
		//changes the corresponding animation speed
		p.currentAnimation->setFrameSpeed(25 / speed);

	// explosions
	for (int i = 0; i < explosions.size(); ++i)
	{
		//increments the explosion timer
		explosionTimer[i] += dt;
		//manual animation?? maybe fix this later
		if (explosionTimer[i] > 0.05 && explosionTimer[i] < 0.1)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion2"));
		}
		if (explosionTimer[i] > 0.1 && explosionTimer[i] < 0.15)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion3"));
		}
		if (explosionTimer[i] > 0.15 && explosionTimer[i] < 0.2)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion4"));
		}
		if (explosionTimer[i] > 0.2 && explosionTimer[i] < .25)
		{
			explosions[i].setTexture(&textMan->getTexture("explosion5"));
		}
		//setting the size of the animation
		explosions[i].setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
		if (explosionTimer[i] > 0.25)
		{
			//erases the explosion
			explosions.erase(explosions.begin() + i);
			//resets the timer
			explosionTimer.erase(explosionTimer.begin() + i);

		}
	}

	// check for collisions
	if (p.isDamaged())
	{
		// if you're damaged you can keep going.
		return;
	}
	//creates entity for kickable boxes
	for (int i = 0; i < kickables.size(); ++i)// k : kickables)
	{
		GameObject k = kickables[i];
		if (colliding(k))
		{
			if (p.isKicking())
			{
				// explode the box
				//if kicked at the right time plays success sound
				kickables.erase(kickables.begin() + i);
				audio->playSoundbyName("success");
				GameObject explosion;
				//sets the size of the explosion object
				explosion.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
				explosion.setPosition(p.getPosition());
				//sets the texture to boom 1
				explosion.setTexture(&textMan->getTexture("explosion1"));
				//explosion and timer reset
				explosions.push_back(explosion);
				explosionTimer.push_back(0.f);
			} 
			else
			{
				//otherwise you miss the kick and it hits you
				hits++;
				//damage increases by 1
				p.setDamaged(1);
				//sets speed to 0
				speed = 0.f;
				// delete object so you don't collide with it again.
				kickables.erase(kickables.begin()+i);
				audio->playSoundbyName("death");
			}
		}
	}
	// check for collisions
	for (int i = 0; i < jumpables.size(); ++i)
	{
		GameObject j = jumpables[i];
		//checks if the player collides with jumpable boxes
		if (colliding(j))
		{
			//same as explodables 
			hits++;
			p.setDamaged(1);
			speed = 0.f;
			jumpables.erase(jumpables.begin() + i);
			audio->playSoundbyName("death");
		}
	}

}

void RunnerLevel::render()
{
	//draws all the entities and objects for the level to render
	beginDraw();
	for(GameObject bg : BGs) window->draw(bg);
	window->draw(moon);
	for (GameObject j : jumpables) window->draw(j);
	for (GameObject k : kickables) window->draw(k);
	window->draw(finishLine);
	window->draw(p);
	window->draw(progressLine);
	window->draw(destinationPoint);
	window->draw(progressP);
	for (GameObject e : explosions) window->draw(e);
	window->draw(moon);
	endDraw();
}

void RunnerLevel::reset()
{
	//for reset you essnetially reuse the constructor to reset all the parameters of the level

	//resets the random orientation of the level and all variables
	std::srand(static_cast<unsigned>(std::time(nullptr)));
	hits = 0;
	time = 0.f;
	objects = 0.f;
	travelled = 0.f;

	// setup Player
	p.setPosition(30, window->getSize().y * 0.6);
	speed = 0.f;
	//clears the kickable and jumpable objects
	BGs.clear();
	kickables.clear();
	jumpables.clear();

	// setup BGs as ten images next to each other. base dimensions 1024x1024
	float bgScalar = window->getSize().y / 1024.0f;
	for (int i = 0; i < 21; ++i)
	{
		GameObject bg;
		bg.setTexture(&textMan->getTexture("bg_Scroll"));
		bg.setSize(sf::Vector2f(1024.0f * bgScalar, 1024.0f * bgScalar));
		bg.setPosition(i * 1024 * bgScalar, 0);
		BGs.push_back(bg);
	}

	float placementIndex = 0.f;
	while (placementIndex < distance)
	{
		// go forward a random distance:
		placementIndex += getRandomInt(650, 800);
		// harder in the back half.
		if (placementIndex > distance / 2) placementIndex -= getRandomInt(100, 250);
		if (placementIndex > distance) break;
		objects += 1;
		GameObject newObj;
		newObj.setPosition(placementIndex, window->getSize().y * 0.6);
		newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.1));
		// randomly put a jumpable, kickable or two-jumpables next to each other
		int seed = getRandomInt(0, 2);
		switch (seed)
		{
		case 0:
			// kickable
			newObj.setTexture(&textMan->getTexture("kickable"));
			newObj.setPosition(placementIndex, window->getSize().y * 0.5);
			newObj.setSize(sf::Vector2f(window->getSize().y * 0.1, window->getSize().y * 0.2));
			kickables.push_back(newObj);
			break;
		case 1:
			// jumpable
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			break;
		case 2:
			newObj.setTexture(&textMan->getTexture("jumpable"));
			jumpables.push_back(newObj);
			GameObject secondObj = newObj;
			secondObj.setPosition(secondObj.getPosition().x + secondObj.getSize().x, secondObj.getPosition().y);
			jumpables.push_back(secondObj);
			break;
		}
	}
	finishLine.setPosition(distance, window->getSize().y * 0.5);
	p.setDamaged(0.5);
}