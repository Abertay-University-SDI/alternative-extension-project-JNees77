#include "TransitionLevel.h"

TransitionLevel::TransitionLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	// store parameters in local references.
	window = hwnd;
	input = in;
	textMan = tm;
	audio = aud;
	gameState = gs;

	// set the text object
	font.loadFromFile("font/montS.ttf");
	explain.setFont(font);
	explain.setPosition(sf::Vector2f(10, 10));
	explain.setCharacterSize(40);

	// initialise background. base size: 5760, 3240
	bg.setTexture(&textMan->getTexture("redSkyBG"));

	cursor = Cursor(window, input);
	returnButton = Button(window, input);
	returnButton.setPositioning(sf::Vector2f(window->getSize().x * 0.9, window->getSize().y * 0.1));
	returnButton.setSize(sf::Vector2f(window->getSize().x * 0.15, window->getSize().y * 0.075));
	buttonTxt.setFont(font);
	buttonTxt.setString("Return");
	buttonTxt.setCharacterSize(20);
	buttonTxt.setOutlineThickness(3.f);
	buttonTxt.setPosition(returnButton.getPosition().x - returnButton.getSize().x / 2, returnButton.getPosition().y - returnButton.getSize().y / 2);
}

TransitionLevel::~TransitionLevel()
{
}

void TransitionLevel::handleInput(float dt)
{
	//once the player decides to start the level it will use the switch case 
	//to select to play out the state the player should see
	if (input->isPressed(sf::Keyboard::Enter))
	{
		switch (gameState->getCurrentState())
		{
		case State::PRE_ONE:
			gameState->setCurrentState(State::LEVEL);
			break;
		case State::PRE_TWO:
			gameState->setCurrentState(State::RUNNER);
			break;
		case State::PRE_THREE:
			gameState->setCurrentState(State::WIZARD);
			break;
			//this one is when you beat a level
		case State::ENDGAME:
			gameState->setCurrentState(State::RESET);
			break;
		}
	}

	if (Collision::checkBoundingBox(&returnButton, &cursor) && input->isRightMouseDown())
	{
		gameState->setCurrentState(State::TITLE);
		gameState->setSingleRun(false);
	}
}

void TransitionLevel::update(float dt)
{
    bg.setSize(sf::Vector2f(window->getView().getSize().x, window->getView().getSize().y));

	cursor.update(dt);
	returnButton.update(dt);
	// set text object by state.
	//this defines which instructions will be shown to the player before they begin a certain game
	switch (gameState->getCurrentState())
	{
	case State::PRE_ONE:
		explain.setString("Move with WASD.\nMove in time with the claps.\nAvoid tanks and pits.\nThey are metaphors for adversary.\nThere is a checkpoint halfway round.\nIt will be yellow when enabled.\nYou cannot pause.\nYou will have assistance.\nYou will be motivated.\nIt will not be helpful.\nHit Enter.");
		break;
	case State::PRE_TWO:
		explain.setString("Kick (enter) wooden boxes.\nJump over (space) metal ones.\nYou got this.\nGood Luck.\nHit Enter");
		break;
	case State::PRE_THREE:
		explain.setString("Move with WASD.\nMove in time with the claps.\nAvoid Tanks and Pits.\nThere is a checkpoint.\nYou will be assisted.\nKind of.\nHit Enter.\nGood Luck.");
		break;
	case State::ENDGAME:
		//this stores the results for the user once they reach the required area
		runResults* results = gameState->getResults();
		std::string resultsString;
		if (results->L1Time > 0)
		{
			resultsString += "Level 1 deaths:\t\t\t" + std::to_string(results->L1Deaths) + "\n";
			resultsString += "Level 1 time:\t\t\t" + std::to_string(results->L1Time) + " seconds\n\n";
		}
		if (results->L2Time > 0)
		{
			resultsString += "Level 2 mistakes:\t\t\t" + std::to_string(results->L2Deaths) + "\n";
			resultsString += "Level 2 time:\t\t\t" + std::to_string(results->L2Time) + " seconds\n\n";
		}
		if (results->L3Time > 0)
		{
			resultsString += "Level 3 deaths:\t\t\t" + std::to_string(results->L3Deaths) + "\n";
			resultsString += "Level 3 time:\t\t\t" + std::to_string(results->L3Time) + " seconds\n";
		}
		explain.setString(resultsString + "\nPress Enter to continue.");
		break;
	}

}

void TransitionLevel::render()
{
	//renders the objects to be drawn
	beginDraw();
	window->draw(bg);
	window->draw(explain);
	returnButton.render(window);
	window->draw(buttonTxt);
	
	endDraw();
}
