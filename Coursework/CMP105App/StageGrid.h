#pragma once

#include <SFML/Graphics.hpp>
#include "Random"
#include "TextureManager.h"
#include "Framework/GameObject.h"

class StageGrid
{
public:
	StageGrid();
	//overloaded constructor that takes the dimension of the object you working with in this case it seems to be the game window
	StageGrid(sf::Vector2i dimensions, float cellSize, sf::Vector2f position, sf::Vector2i start, sf::Vector2i end, sf::Vector2i cp, int stage, TextureManager* tm);

	enum cellState
	{
		//all the elements you could interact with within a grid
		SAFE,
		PIT,			// a hazard that does not move.
		HAZARD_UP,
		HAZARD_DOWN,
		HAZARD_LEFT,
		HAZARD_RIGHT,
		START,
		END,
		CHECKPOINT
	};

	void update(int frames);
	void render(sf::RenderWindow* wnd, bool cp_on);
	//check to see if the player is hit by one of the objects
	bool playerHit(std::pair<int, int> pos);

private:
	TextureManager* textMan;
	std::vector<std::vector<cellState>> grid;
	std::vector<std::vector<GameObject>> board;
	//takes the size of the cells
	float cellSize;
	sf::Vector2f position;
};


