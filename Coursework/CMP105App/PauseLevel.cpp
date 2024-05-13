#include "PauseLevel.h"

PauseLevel::PauseLevel()
{

}
PauseLevel::PauseLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, TextureManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	textMan = tm;

	fan.setPosition(window->getSize().x * 0.7, window->getSize().y * 0.5);
	float fanProportion = textMan->getProportions("sadBear");
	fan.setSize(sf::Vector2f(window->getSize().x * 0.2, window->getSize().x * 0.2 * fanProportion));
	fan.setTexture(&textMan->getTexture("sadBear"));

	bg.setTexture(&textMan->getTexture("titleBG"));

	x = window->getSize().x/2;
	y = window->getSize().y/2;

	font.loadFromFile("font/montS.ttf");

	for (int i = 0; i < 2; i++)
	{
		button[i] = Button(window, input, sf::Vector2f(x / 3, y / 5), sf::Vector2f(x - button[i].getSize().x / 2, (i + 1) * 200));
		text[i].setFont(font);
		text[i].setCharacterSize(24);
		text[i].setOutlineThickness(3.f);
	}
	text[0].setPosition(button[0].getPosition().x - button[1].getSize().x / 2.5, button[0].getPosition().y - button[0].getSize().y / 8);
	text[1].setPosition(button[1].getPosition().x - button[1].getSize().x / 4, button[1].getPosition().y - button[1].getSize().y / 8);
	cursor = Cursor(window, input);

	text[0].setString("Back to Titlescreen");
	text[1].setString("Main Menu");
}
PauseLevel::~PauseLevel()
{

}
void PauseLevel::update(float dt)
{
	bg.setSize(sf::Vector2f(window->getView().getSize().x, window->getView().getSize().y));
	fan.setPosition(window->getSize().x * 0.1, window->getSize().y * 0.6);
	float fanProportion = textMan->getProportions("sadBear");
	fan.setSize(sf::Vector2f(window->getSize().x * 0.2, window->getSize().x * 0.2 * fanProportion));

	textMan->animate(&fan, "sadBear", animTimer);
	animTimer += dt;

	cursor.update(dt);
	for (int i = 0; i < 2; i++)
	{
		button[i].update(dt);
	}
}
void PauseLevel::handleInput(float dt)
{
	if (Collision::checkBoundingBox(&button[0], &cursor) && input->isLeftMouseDown())
	{
		gameState->setCurrentState(State::TITLE);
		Sleep(500);
	}
	if (Collision::checkBoundingBox(&button[1], &cursor) && input->isLeftMouseDown())
	{
		gameState->setCurrentState(State::MENU);
		Sleep(500);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
	{
		gameState->setCurrentState(gameState->getPreviousState());
		Sleep(500);
	}
}
void PauseLevel::render()
{
	beginDraw();
	window->draw(bg);
	for (int i = 0; i < 2; i++)
	{
		button[i].render(window);
		window->draw(text[i]);
	}
	window->draw(fan);
	endDraw();
}