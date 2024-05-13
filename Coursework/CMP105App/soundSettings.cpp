#include "soundSettings.h"

soundSettings::soundSettings()
{

}
soundSettings::soundSettings(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	window = hwnd;
	input = in;
	textMan = tm;
	audio = aud;
	
	font.loadFromFile("font/montS.ttf");

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
	text[0].setString("Volume");
	text[1].setString("Tone");

	cursor = Cursor(window, input);

	minVol = 390.f;
	minPitch = 390.f;
}
soundSettings::~soundSettings()
{

}
void soundSettings::update(float dt)
{
	cursor.update(dt);
	for (int i = 0; i < 2; i++)
	{
		bar[i].update(dt);
		dial[i].update(dt);
	}

	volume = dial[0].getPosition().x - (minVol + bar[0].getSize().x - bar[0].getPosition().x);
	audio->setMusicVolume(volume/8);
	pitch = dial[1].getPosition().x - (minPitch + bar[1].getSize().x - bar[1].getPosition().x);
	audio->setMusicPitch(pitch / 800);
}
void soundSettings::handleInput(float dt)
{
	if (Collision::checkBoundingBox(&dial[0], &bar[0]) && Collision::checkBoundingBox(&bar[0], &cursor) && input->isLeftMouseDown())
	{
		dial[0].setPosition(cursor.getPosition().x, bar[0].getPosition().y);
	}
	if (Collision::checkBoundingBox(&dial[1], &bar[1]) && Collision::checkBoundingBox(&bar[1], &cursor) && input->isLeftMouseDown())
	{
		dial[1].setPosition(cursor.getPosition().x, bar[1].getPosition().y);
	}
}
void soundSettings::render()
{
	beginDraw();
	for (int i = 0; i < 2; i++)
	{
		window->draw(bar[i]);
		window->draw(dial[i]);
		window->draw(text[i]);
	}
	endDraw();
}
