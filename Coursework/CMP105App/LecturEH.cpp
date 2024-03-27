#include "LecturEH.h"

//This creates the animation for the wizard mini game
LecturEH::LecturEH(TextureManager* tm)
{
	//sets the texture 
	setTexture(&tm->getTexture("lecturEH"));
	//adds the frames
	still.addFrame(sf::IntRect(0, 0, 32, 32));
	still.setFrameSpeed(1.f);

	hop.addFrame(sf::IntRect(32, 0, 32, 32));
	hop.addFrame(sf::IntRect(96, 0, 32, 32));
	hop.addFrame(sf::IntRect(64, 0, 32, 32));
	hop.addFrame(sf::IntRect(96, 0, 32, 32));
	hop.setFrameSpeed(1.f / 4.f);
	//plays the current chosen animation
	currentAnimation = &still;
}

LecturEH::~LecturEH()
{
}

void LecturEH::update(float dt)
{
	//calulates the time for each hop
	timeSinceHop += dt;
	//checks how long its occured
	if (timeSinceHop > HOP_DELAY + 1)
	{
		//resets the hop animation
		// hop over.
		hop.reset();
		timeSinceHop = 0.f;
		//plays the still animation
		currentAnimation = &still;
	}
	else if (timeSinceHop > HOP_DELAY)
	{
		currentAnimation = &hop;
	}
	//animates the current animation

	currentAnimation->animate(dt);
	setTextureRect(currentAnimation->getCurrentFrame());
}

