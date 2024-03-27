#include "Background.h"

Background::Background()
{
	//background texture (change later)
	bg.loadFromFile("gfx/Level1_1.png");
	//set the background texture
	setTexture(&bg);
	//setting it to the size of the screen
	setSize(sf::Vector2f(11038, 675));
}

void Background::setWindow(sf::RenderWindow* wnd)
{
	//setting the window
	window = wnd;
}

void Background::handleInput(float dt)
{
	//gets the view
	sf::View v = window->getView();
	//checks if the space bar is pressed
	if (input->isKeyDown(sf::Keyboard::Space))
	{
		//sets the view as the prevoius view plus five in the x direction
		v.setCenter(v.getCenter().x + 5, v.getCenter().y);
	} 
	//otherwise the view is the same as the window
	window->setView(v);
}