#include "Level.h"
#include <iostream>
//playerPosition
Level::Level(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm)
{
	window = hwnd;
	input = in;
	gameState = gs;
	audio = aud;
	textMan = tm;

	// create the lecturer object.
	lecturer = Lecturer(window, textMan);

	// initialise game objects
	selectedAction = NONE;
	beatsPlayed = 0;

	// initialise background. base size: 5760, 3240
	//This is the background from the start of the game
	levelBG.setTexture(&textMan->getTexture("redSkyBG"));
	float bgScalar = std::max(hwnd->getSize().x / 5760.f, hwnd->getSize().y / 3240.f);
	levelBG.setSize(sf::Vector2f(5760*bgScalar, 3240*bgScalar));

	// initialise grid board
	// .. first calculate size. 
	//gets the window proportions
	float windowWidth = window->getSize().x;
	float windowHeight = window->getSize().y;

	TextBox.setPosition(sf::Vector2f(window->getSize().x * 0.5, window->getSize().y * 0.66));
	TextBox.setSize(sf::Vector2f(window->getSize().x * 0.4, window->getSize().y * 0.2));
	TextBox.setFillColor(sf::Color::Black);
	TextBox.setOutlineColor(sf::Color::Magenta);
	TextBox.setOutlineThickness(3.f);

	boardTop = windowHeight * 0.05;	// the board is as wide as can be with the given constants.
	boardLeft = boardTop;	// uniform top and side padding.
	boardRight = windowWidth - boardLeft;
	int numRows = 10;
	int numCols = 20;
	//divides the number of steps that can be taken by 20*
	cellDim = (boardRight - boardLeft) / numCols;
	boardBottom = numRows * cellDim + boardTop;
	// ensure grid does not take up too much of the sapce
	float maxHeight = boardTop + windowHeight * GRID_Y_MAX_PROPORTION;
	while (boardBottom > maxHeight)
	{
		// reduce cell size by 2px, reducing cells by 1px from each edge anchored at top. gridTop remains constant.
		cellDim -= 2;
		boardLeft += numCols;
		boardRight -= numCols;
		boardBottom -= numRows * 2;
	}

	// .. Then setup board
	gridBoard.setFillColor(sf::Color::White);
	gridBoard.setSize(sf::Vector2f(boardRight - boardLeft, boardBottom - boardTop));
	gridBoard.setPosition(boardLeft, boardTop);

	// setup player component.
	playerPosition = { start.x, start.y };
	//sets the players position to the grid board position + the dimensions of the cell (uniform)
	//then multpilied by the vector value of start (0,3)
	player.setPosition(sf::Vector2f(
		gridBoard.getPosition().x + cellDim * start.x,
		gridBoard.getPosition().y + cellDim * start.y)
	);
	//player is uniform size (the calc for cellDim is above)
	player.setSize(sf::Vector2f(cellDim, cellDim));

	// Setup progress bar component.
	//this is the bar that charges to time the key presses
	progressInStep.setPosition(sf::Vector2f(900, 800));
	progressInStep.setSize(sf::Vector2f(0, 200));
	progressInStepBG.setPosition(sf::Vector2f(900, 790));
	progressInStepBG.setSize(sf::Vector2f(500, 220));
	progressInStepBG.setFillColor(sf::Color::Black);
	//this calculation tracks to see if the bar is in the right zone (when it turns green)
	targetZone.setPosition(sf::Vector2f(
		progressInStepBG.getPosition().x + progressInStepBG.getSize().x * (TIME_PER_STEP - TIME_FOR_ACTION - TIME_BUFFER) / TIME_PER_STEP,
		790
	));
	//this is the area where you want to aim ur key presses
	targetZone.setSize(sf::Vector2f(
		progressInStepBG.getSize().x * (TIME_FOR_ACTION / TIME_PER_STEP),
		220
	));
	targetZone.setFillColor(sf::Color::Blue);

	// setup controls component.
	//This shows the background for the controls to sit in
	font.loadFromFile("font/montS.ttf");
	controls.push_back(sf::Text());
	controls[0].setPosition(boardLeft, boardBottom);
	controls[0].setCharacterSize(30);
	controls[0].setFont(font);
	controlBG.setPosition(boardLeft, boardBottom);
	controlBG.setSize(sf::Vector2f(380, 280));
	controlBG.setFillColor(sf::Color::Red);

	changeX = cellDim / 20;
	changeY = cellDim / 20;

	// setup grid component.
	grid = StageGrid(
		sf::Vector2i(numCols, numRows), 
		cellDim, 
		gridBoard.getPosition(), 
		start, 
		end, 
		checkPoint,
		1,	
		textMan
	);

	// setup indicators
	//sets names for the indicators (code)
	std::vector<std::string> indicatorNames = { "upIcon", "rightIcon", "downIcon", "leftIcon" };
	//sets the displayed version for the user
	std::vector<char> indicatorLabels = { 'W', 'D', 'S', 'A'};
	//uses the indicator names to set the texture
	for (int i = 0; i < indicatorNames.size(); ++i)
	{
		GameObject icon;
		icon.setTexture(&textMan->getTexture(indicatorNames[i]));
		icon.setSize(sf::Vector2f(80, 80));
		icon.setPosition(boardLeft + i*100, boardBottom + 100);
		indicators.push_back(icon);
		sf::Text iconText;
		iconText.setFont(font);
		iconText.setString(indicatorLabels[i]);
		iconText.setPosition(boardLeft + i * 100 + 28, boardBottom + 200);
		controls.push_back(iconText);
	}

	// set state.
	checkPointEnabled = false;

	// set alert
	//text object
	alert.setFont(font);
	//sets the position the text will be displayed at
	alert.setPosition(window->getSize().x * 0.4, boardBottom);
	//sets the size of the characters
	alert.setCharacterSize(50);
	//ses the colour
	alert.setFillColor(sf::Color::Yellow);

}

Level::~Level()
{

}

// handle user input
void Level::handleInput(float dt)
{
	player.setPosition(sf::Vector2f(
		gridBoard.getPosition().x + cellDim * playerPosition.first,
		gridBoard.getPosition().y + cellDim * playerPosition.second)
	);

	if (damagedTimer > 0)
	{
		damagedTimer -= dt;
		return;
	}
	damagedTimer = 0.f;

	// check input has come at suitable point in progress.
	float timeLeft = TIME_PER_STEP - timeInStep;
	if (timeLeft > TIME_BUFFER && timeLeft < TIME_BUFFER + TIME_FOR_ACTION && selectedAction != FAIL)
	{
		tim = clock.restart();
		distance = speed * timeCondition;
		//depending on the action and if pressed at the right time displays one of the following
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			selectedAction = LEFT;
			//flips the players animation
			player.setFlipped(true);


			clock.restart();
			timeCondition = 0.05f;
			while (tim.asSeconds() < 1.f)
			{
				tim = clock.getElapsedTime();

				if (tim.asSeconds() > timeCondition)
				{
					player.setPosition((gridBoard.getPosition().x + changeX), player.getPosition().y);
					timeCondition += 0.05f;
					changeX -= cellDim / 20;
				}
				update(dt);
				render();
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			selectedAction = UP;

			clock.restart();
			timeCondition = 0.05f;
			while (tim.asSeconds() < 1.f)
			{
				tim = clock.getElapsedTime();

				if (tim.asSeconds() > timeCondition)
				{
					player.setPosition(player.getPosition().x, (gridBoard.getPosition().y + changeY + (cellDim*3)));
					timeCondition += 0.05f;
					changeY -= cellDim / 20;
				}
				update(dt);
				render();
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			selectedAction = RIGHT;
			//flips the players position back the other way
			player.setFlipped(false);
			//CHANGE
			clock.restart();
			timeCondition = 0.05f;
			while (tim.asSeconds() < 1.f)
			{
				tim = clock.getElapsedTime();
				
				if (tim.asSeconds() > timeCondition)
				{
					player.setPosition((gridBoard.getPosition().x + changeX), player.getPosition().y);
					timeCondition += 0.05f;
					changeX += cellDim/20;
				}
				update(dt);
				render();
			}

		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			selectedAction = DOWN;

			clock.restart();
			timeCondition = 0.05f;
			while (tim.asSeconds() < 1.f)
			{
				tim = clock.getElapsedTime();

				if (tim.asSeconds() > timeCondition)
				{
					player.setPosition(player.getPosition().x, (gridBoard.getPosition().y + changeY) + (cellDim * 3));
					timeCondition += 0.05f;
					changeY += cellDim / 20;
				}
				update(dt);
				render();
			}
		}
		alert.setString("");
	}
	else 
	{
		// Timing incorrect, any attempted input is a fail.
		if (selectedAction == NONE)	misses++;	// don't increment misses more than once / step
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			selectedAction = FAIL;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			selectedAction = FAIL;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			selectedAction = FAIL;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			selectedAction = FAIL;
		}
		
	}
}

// Update game objects
void Level::update(float dt)
{
	// display alert or reset alert.
	if (alert.getString() != "" && alertHasBeenActiveFor < TIME_TO_DISPLAY_ALERT)
	{
		//displas the alert for 0.5 seconds
		alertHasBeenActiveFor += dt;
	}
	else
	{
		//sets the alert to a blank char value for 0,5s
		alert.setString("");
		alertHasBeenActiveFor = TIME_TO_DISPLAY_ALERT;
	}

	// check for win
	if (playerPosition.first == end.x && playerPosition.second == end.y)
	{
		//uses game state, specifically for the wizard game to count the number of deaths in the wizard game and the time taken
		gameState->addResult("l1deaths", deaths);
		gameState->addResult("l1time", timeTaken);
		//checks to see if the level has been cleared and shows the winning screen
		if (gameState->getSingleRun()) gameState->setCurrentState(State::ENDGAME);
		//otherwise sets the current start to whatever that is*
		else gameState->setCurrentState(State::PRE_TWO);
		//plays success audio
		audio->playSoundbyName("success");
	}
	else
	{
		//increments time taken
		timeTaken += dt;
	}

	// check for checkpoint
	if (!checkPointEnabled && playerPosition.second > checkPoint.y)
	{
		//sets checkpoints to true plays a sound, shows an alert
		checkPointEnabled = true;
		audio->playSoundbyName("success");
		alert.setString("checkpoint");
		alertHasBeenActiveFor = 0.f;
	}

	// update references. NOTE: gridboard only updated sometimes (enemies move each step, not each frame).
	player.update(dt);
	lecturer.update(dt);

	// Play beat ONCE per step.
	if (!soundPlayed && timeInStep > TIME_PER_STEP - (TIME_FOR_ACTION / 2 + TIME_BUFFER))
	{
		//sound to keep in beat with
		audio->playSoundbyName("clap");
		soundPlayed = true;
	}
	
	// update UI
	//tells the user if their press was too fast or too slow and presents the message
	for(int i = 0; i < 4; ++i) indicators[i].setFillColor(sf::Color::White);
	switch (selectedAction)
	{
	case UP:
		controls[0].setString("up");
		indicators[0].setFillColor(sf::Color::Yellow);
		break;
	case RIGHT:
		controls[0].setString("right");
		indicators[1].setFillColor(sf::Color::Yellow);
		break;
	case DOWN:
		controls[0].setString("down");
		indicators[2].setFillColor(sf::Color::Yellow);
		break;
	case LEFT:
		controls[0].setString("left");
		indicators[3].setFillColor(sf::Color::Yellow);
		break;
	case FAIL:
		controls[0].setString("FAIL");
		break;
	case NONE:
		controls[0].setString("none");
		break;
	}//getMessageToDisplay

	// update progress component
	//this is the bar that moves that requires you to tap in the blue box (code for blue box above)
	timeInStep += dt;
	progressInStep.setSize(sf::Vector2f((timeInStep / TIME_PER_STEP) * progressInStepBG.getSize().x,200	));
	float timeLeft = TIME_PER_STEP - timeInStep;
	if (timeLeft > TIME_BUFFER && timeLeft < TIME_BUFFER + TIME_FOR_ACTION && selectedAction != FAIL)
	{
		progressInStep.setFillColor(sf::Color::Green);
	}
	else
	{
		progressInStep.setFillColor(sf::Color::Red);
	}

	// movement
	if (timeInStep >= TIME_PER_STEP)
	{
		// do a move.
		beatsPlayed++;
		//passes number of frames
		grid.update(0);
		timeInStep = 0.f;
		soundPlayed = false;

		//depending in the action moves the player
		switch (selectedAction)
		{
		case UP:
			if (playerPosition.second == 0)
			{
				resetPlayer();
				break;
			}
			playerPosition.second--;	// positive-y innit.
			break;
		case RIGHT:
			if (playerPosition.first == boardDimensions.x - 1)
			{
				resetPlayer();
				break;
			}
			playerPosition.first++;
			player.setFlipped(false);
			break;
		case DOWN:
			if (playerPosition.second == boardDimensions.y - 1)
			{
				resetPlayer();
				break;
			}
			playerPosition.second++;
			break;
		case LEFT:
			if (playerPosition.first == 0)
			{
				resetPlayer();
				break;
			}
			playerPosition.first--;
			break;
		}
		//CHANGE
		/*player.setPosition(sf::Vector2f(
			gridBoard.getPosition().x + cellDim * playerPosition.first,
			gridBoard.getPosition().y + cellDim * playerPosition.second)
		);*/
		if (grid.playerHit(playerPosition))
		{
			resetPlayer();
		}
		selectedAction = NONE;

	}
	// update alert for early/late if needed.
	if (alert.getString() == "" && selectedAction == FAIL)
	{
		alertHasBeenActiveFor = 0.f;
		if (timeLeft < TIME_BUFFER)
		{
			// hack: stop alert carrying over in early frames of new step.
			//displays mistake message
			if (alert.getString() != "too fast")
				alert.setString("too slow");
		}
		else
		{
			alert.setString("too fast");
		}
	}
}

// Render level
void Level::render()
{
	beginDraw();
	//draws all the images for the lecturer level
	window->draw(levelBG);
	grid.render(window, checkPointEnabled);
	window->draw(controls[0]);
	window->draw(player);
	window->draw(progressInStepBG);
	window->draw(targetZone);
	window->draw(progressInStep);
	//added
	window->draw(TextBox);
	window->draw(lecturer);
	//if(!lecturer.getMessageToDisplay(boardTop, boardRight, boardBottom, boardLeft).getString().isEmpty())
		//window->draw(lecturer.getMessageToDisplay(boardTop, boardRight, boardBottom, boardLeft));
	if (!lecturer.getMessageToDisplay(TextBox.getPosition().y, TextBox.getPosition().x + TextBox.getSize().x,
		(TextBox.getPosition().y + TextBox.getSize().y), TextBox.getPosition().x).getString().isEmpty());
	{
		window->draw(lecturer.getMessageToDisplay(TextBox.getPosition().y, TextBox.getPosition().x + TextBox.getSize().x,
			TextBox.getPosition().y + TextBox.getSize().y - 40, TextBox.getPosition().x));
	}
	window->draw(controlBG);
	//sizeTextToGrid
	for (GameObject ind : indicators)
	{
		//draws the indicators like (->)
		window->draw(ind);
	}
	for (sf::Text label : controls)
	{
		//draws the labels like (Up)
		window->draw(label);
	}
	//draws the alerts
	window->draw(alert);
	endDraw();
}

void Level::renderPlayer()
{
	beginDraw();
	window->draw(player);
	endDraw();
}

/*
Put player back to the starting space. 
*/
void Level::resetPlayer()
{
	//if the player dies resets everything using this code
	if (checkPointEnabled)
	{
		playerPosition = { checkPoint.x, checkPoint.y };
		changeX = playerPosition.first * cellDim;
		changeY = playerPosition.second * cellDim - (cellDim *3);

	}
	else
	{
		playerPosition = { start.x, start.y };
		changeX = cellDim / 20;
		changeY = cellDim / 20;
	}
	//plays a death sound and increases death counter
	audio->playSoundbyName("death");
	damagedTimer = RESET_TIME;
	player.setDamaged(damagedTimer);
	deaths++;
	
}

void Level::reset()
{
	// create the lecturer object.
	lecturer = Lecturer(window, textMan);

	// initialise game objects
	selectedAction = NONE;
	beatsPlayed = 0;
	timeTaken = 0.f;
	misses = 0;
	deaths = 0;

	// initialise background. base size: 5760, 3240
	levelBG.setTexture(&textMan->getTexture("redSkyBG"));
	float bgScalar = std::max(window->getSize().x / 5760.f, window->getSize().y / 3240.f);
	levelBG.setSize(sf::Vector2f(5760 * bgScalar, 3240 * bgScalar));

	// initialise grid board
	// .. first calculate size. 
	float windowWidth = window->getSize().x;
	float windowHeight = window->getSize().y;
	boardTop = windowHeight * 0.05;	// the board is as wide as can be with the given constants.
	boardLeft = boardTop;	// uniform top and side padding.
	boardRight = windowWidth - boardLeft;
	int numRows = 10;
	int numCols = 20;
	cellDim = (boardRight - boardLeft) / numCols;
	boardBottom = numRows * cellDim + boardTop;
	// ensure grid does not take up too much of the sapce
	float maxHeight = boardTop + windowHeight * GRID_Y_MAX_PROPORTION;
	while (boardBottom > maxHeight)
	{
		// reduce cell size by 2px, reducing cells by 1px from each edge anchored at top. gridTop remains constant.
		cellDim -= 2;
		boardLeft += numCols;
		boardRight -= numCols;
		boardBottom -= numRows * 2;
	}

	start = { 0,3 };
	end = { 0, 8 };
	checkPoint = { 19, 5 };

	playerPosition = { start.x, start.y };

	// .. Then setup board
	gridBoard.setFillColor(sf::Color::White);
	gridBoard.setSize(sf::Vector2f(boardRight - boardLeft, boardBottom - boardTop));
	gridBoard.setPosition(boardLeft, boardTop);

	// setup player component.
	player.setPosition(sf::Vector2f(
		gridBoard.getPosition().x + cellDim * start.x,
		gridBoard.getPosition().y + cellDim * start.y)
	);
	player.setSize(sf::Vector2f(cellDim, cellDim));

	// Setup progress bar component.
	progressInStep.setPosition(sf::Vector2f(900, 800));
	progressInStep.setSize(sf::Vector2f(0, 200));
	progressInStepBG.setPosition(sf::Vector2f(900, 790));
	progressInStepBG.setSize(sf::Vector2f(500, 220));
	progressInStepBG.setFillColor(sf::Color::Black);
	targetZone.setPosition(sf::Vector2f(
		progressInStepBG.getPosition().x + progressInStepBG.getSize().x * (TIME_PER_STEP - TIME_FOR_ACTION - TIME_BUFFER) / TIME_PER_STEP,
		790
	));
	targetZone.setSize(sf::Vector2f(
		progressInStepBG.getSize().x * (TIME_FOR_ACTION / TIME_PER_STEP),
		220
	));
	targetZone.setFillColor(sf::Color::Blue);

	// setup grid component.
	grid = StageGrid(
		sf::Vector2i(numCols, numRows),
		cellDim,
		gridBoard.getPosition(),
		start,
		end,
		checkPoint,
		1,
		textMan
	);

	// set state.
	checkPointEnabled = false;

	// set alert
	alert.setFont(font);
	alert.setPosition(window->getSize().x * 0.4, boardBottom);
	alert.setCharacterSize(50);
	alert.setFillColor(sf::Color::Yellow);
}
