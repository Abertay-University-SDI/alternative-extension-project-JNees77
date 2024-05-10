#include "PauseLevel.h"

PauseLevel::PauseLevel()
{

}
PauseLevel::PauseLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs)
{
	window = hwnd;
	input = in;
	gameState = gs;

	x = window->getSize().x/2;
	y = window->getSize().y/2;

	font.loadFromFile("font/montS.ttf");

	for (int i = 0; i < 2; i++)
	{
		button[i] = Button(window, input, sf::Vector2f(x / 12, y / 16), sf::Vector2f(x - button[i].getSize().x / 2, (i + 1) * 200));
		text[i].setFont(font);
		text[i].setCharacterSize(24);
		text[i].setOutlineThickness(3.f);
		text[i].setPosition(button[i].getPosition().x - button[i].getSize().x / 4, button[i].getPosition().y - button[i].getSize().y / 8);
	}

	cursor = Cursor(window, input);

	text[0].setString("Back to Titlescreen");
	text[1].setString("Main Menu");
}
PauseLevel::~PauseLevel()
{

}
void PauseLevel::update(float dt)
{
	cursor.update(dt);
	for (int i = 0; i < 2; i++)
	{
		button[i].update(dt);
	}
}
void PauseLevel::handleInput(float dt)
{
	if (Collision::checkBoundingBox(&button[0], &cursor) && input->isRightMouseDown())
	{
		gameState->setCurrentState(State::TITLE);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
	{
		gameState->setCurrentState(gameState->getPreviousState());
	}
}
void PauseLevel::render()
{
	beginDraw();
	for (int i = 0; i < 2; i++)
	{
		button[i].render(window);
		window->draw(text[i]);
	}
	endDraw();
}