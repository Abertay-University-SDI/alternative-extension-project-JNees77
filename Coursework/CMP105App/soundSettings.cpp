#include "soundSettings.h"

//base constructor
soundSettings::soundSettings()
{

}
//overloaded constructor
soundSettings::soundSettings(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	//passes properties from main
	window = hwnd;
	input = in;
	gameState = gs;
	textMan = tm;
	audio = aud;
	//draw fan animation
	fan.setPosition(window->getSize().x * 0.7, window->getSize().y * 0.5);
	float fanProportion = textMan->getProportions("sadBear");
	fan.setSize(sf::Vector2f(window->getSize().x * 0.2, window->getSize().x * 0.2 * fanProportion));
	fan.setTexture(&textMan->getTexture("sadBear"));

	bg.setTexture(&textMan->getTexture("titleBG"));

	font.loadFromFile("font/montS.ttf");
	//sets up text and buttons
	for (int i = 0; i < 2; i++)
	{
		bar[i] = Button(window, input, sf::Vector2f(800, 150), sf::Vector2f(window->getSize().x / 2 - bar[i].getSize().x, (i + 1) * 300));
		dial[i] = Button(window, input, sf::Vector2f(100, 200), sf::Vector2f(bar[i].getPosition()));
		dial[i].setFillColor(sf::Color::Black);
		text[i].setFont(font);
		text[i].setPosition(bar[i].getPosition().x - bar[i].getSize().x / 2, bar[i].getPosition().y - 200);
		text[i].setCharacterSize(32);
		text[i].setOutlineColor(sf::Color::Black);
	}
	//back button properties (to go back to the pause menu)
	back = Button(window, input, sf::Vector2f(window->getSize().x * 0.15, window->getSize().y * 0.075), sf::Vector2f(window->getSize().x * 0.9, window->getSize().y * 0.1));
	//text properties for back button
	returnTxt.setFont(font);
    returnTxt.setString("Return");
	returnTxt.setCharacterSize(20);
	returnTxt.setOutlineThickness(3.f);
	returnTxt.setPosition(back.getPosition().x - back.getSize().x / 4, back.getPosition().y - back.getSize().y / 4);

	cursor = Cursor(window, input);
	//set to the position where the collision boxes start
	minVol = 395.f;
	minPitch = 395.f;
}
soundSettings::~soundSettings()
{

}
void soundSettings::update(float dt)
{
	cursor.update(dt);
	back.update(dt);
	//sets background size and fan animation
	bg.setSize(sf::Vector2f(window->getView().getSize().x, window->getView().getSize().y));
	fan.setPosition(window->getSize().x * 0.1, window->getSize().y * 0.6);
	float fanProportion = textMan->getProportions("sadBear");
	fan.setSize(sf::Vector2f(window->getSize().x * 0.2, window->getSize().x * 0.2 * fanProportion));

	textMan->animate(&fan, "sadBear", animTimer);
	animTimer += dt;
	//update fucntions for buttons
	for (int i = 0; i < 2; i++)
	{
		bar[i].update(dt);
		dial[i].update(dt);
	}

	//sets up the volume and pitch using the scroll bars
	volume = dial[0].getPosition().x - (minVol + bar[0].getSize().x - bar[0].getPosition().x);
	audio->setMusicVolume(round(volume/8));
	pitch = dial[1].getPosition().x - (minPitch + bar[1].getSize().x - bar[1].getPosition().x);
	audio->setMusicPitch((pitch / 400));
	//text for each bar
	text[0].setString("Volume: " + std::to_string(int(volume/8)));
	text[1].setString("Tone: " + std::to_string(int(pitch/400)));
	
}
void soundSettings::handleInput(float dt)
{
	//what happens depending on the button you press
	if (Collision::checkBoundingBox(&dial[0], &bar[0]) && Collision::checkBoundingBox(&bar[0], &cursor) && input->isLeftMouseDown())
	{
		dial[0].setPosition(cursor.getPosition().x, bar[0].getPosition().y);
	}
	if (Collision::checkBoundingBox(&dial[1], &bar[1]) && Collision::checkBoundingBox(&bar[1], &cursor) && input->isLeftMouseDown())
	{
		dial[1].setPosition(cursor.getPosition().x, bar[1].getPosition().y);
	}
	if (Collision::checkBoundingBox(&back, &cursor) && input->isLeftMouseDown())
	{
		gameState->setCurrentState(State::PAUSE);
	}
}
void soundSettings::render()
{
	beginDraw();
	window->draw(bg);
	for (int i = 0; i < 2; i++)
	{
		//draw collison boxes
		bar[i].render(window);
		window->draw(dial[i]);
		window->draw(text[i]);
	}
	back.render(window);
	window->draw(returnTxt);
	window->draw(fan);
	endDraw();
}
