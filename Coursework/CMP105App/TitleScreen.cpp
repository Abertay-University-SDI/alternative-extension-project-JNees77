#include "TitleScreen.h"

TitleScreen::TitleScreen(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	textMan = tm;
	// set positions and textures.
	//   .. for the bear
	fan.setPosition(window->getSize().x * 0.7, window->getSize().y * 0.5);
	float fanProportion = textMan->getProportions("sadBear");
	fan.setSize(sf::Vector2f(window->getSize().x * 0.2, window->getSize().x * 0.2 * fanProportion));
	fan.setTexture(&textMan->getTexture("sadBear"));
	
	// .. for the text
	montsFont.loadFromFile("font/montS.ttf");
	explanationText.setFont(montsFont);
	explanationText.setString("The Forbidden Rhythms.\n\tYou must unleash them.\n\t\tYour fans are waiting.\n\t\t\tHit a button to\n\t\t\t\tBegin.");
	explanationText.setPosition(window->getSize().x * 0.3, window->getSize().y * 0.2);
	explanationText.setCharacterSize(40);
	titleText.setFont(montsFont);
	titleText.setString("FORBIDDEN\n\tRHYTHMS");
	titleText.setPosition(10, window->getSize().y * 0.2);
	titleText.setCharacterSize(60);

	x = window->getSize().x / 1.25;
	for (int i = 0; i < 4; i++)
	{
		buttons[i] = Button(window, in, sf::Vector2f(400,100), sf::Vector2f(x, (i + 1) * 200));
		text[i].setFont(montsFont);
		text[i].setCharacterSize(24);
		text[i].setOutlineThickness(3.f);
		text[i].setFillColor(sf::Color::White);
	}
	
	buttons[3].setSize(sf::Vector2f(125, 75));
	buttons[3].setPosition(sf::Vector2f(window->getSize().x * 0.95, window->getSize().y * 0.05));
	cursor = Cursor(window, in);

	text[0].setString("Practice LECTURER");
	text[1].setString("Practice RUNNER");
	text[2].setString("Practice WIZARD");
	text[3].setString("Exit");

	// .. background
	// initialise background. base size: 1920, 1080
	bg.setTexture(&textMan->getTexture("titleBG"));

	animTimer = 0.0f;
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::handleInput(float dt)
{
	if (Collision::checkBoundingBox(&buttons[0], &cursor) && input->isLeftMouseDown())
	{
		gameState->setCurrentState(State::PRE_ONE);
		gameState->setSingleRun(false);
	}
	if (Collision::checkBoundingBox(&buttons[1], &cursor) && input->isLeftMouseDown())
	{
		gameState->setCurrentState(State::PRE_TWO);
		gameState->setSingleRun(false);
	}
	if (Collision::checkBoundingBox(&buttons[2], &cursor) && input->isLeftMouseDown())
	{
		gameState->setCurrentState(State::PRE_THREE);
		gameState->setSingleRun(false);
	}
	if (Collision::checkBoundingBox(&buttons[3], &cursor) && input->isLeftMouseDown())
	{
		exit(0);
	}

}

void TitleScreen::ButtonSetup()
{
	
	for (int i = 0; i < 3; i++)
	{
		//buttons[i].setPositioning(sf::Vector2f( x, (i + 1) * 200));
		text[i].setPosition(buttons[i].getPosition().x - buttons[i].getSize().x/4, buttons[i].getPosition().y - buttons[i].getSize().y / 8);
	}
	//buttons[3].setPositioning(sf::Vector2f(window->getSize().x * 0.95, window->getSize().y * 0.05));
	text[3].setPosition(buttons[3].getPosition().x - buttons[3].getSize().x / 4, buttons[3].getPosition().y - buttons[3].getSize().y / 8);
}

void TitleScreen::update(float dt)
{
	// update sizes to be dynamic.
	bg.setSize(sf::Vector2f(window->getView().getSize().x, window->getView().getSize().y));
	fan.setPosition(window->getSize().x * 0.1, window->getSize().y * 0.6);
	float fanProportion = textMan->getProportions("sadBear");
	fan.setSize(sf::Vector2f(window->getSize().x * 0.2, window->getSize().x * 0.2 * fanProportion));

	textMan->animate(&fan, "sadBear", animTimer);
	animTimer += dt;

	ButtonSetup();
	for (int i = 0; i < 4; i++)
	{
		buttons[i].update(dt);
	}

	cursor.update(dt);
	
}

void TitleScreen::render()
{
	beginDraw();
	window->draw(bg);
	for (int i = 0; i < 4; i++)
	{
		buttons[i].render(window);
		window->draw(text[i]);
	}
	window->draw(fan);
	window->draw(explanationText);
	window->draw(titleText);
	
	endDraw();
}
