#pragma once

#include "Framework/GameObject.h"
#include "TextureManager.h"
#include "Framework/Animation.h"
#include "StageGrid.h"

class Lecturer : public GameObject
{
public:
	//two constructors
	Lecturer();
	Lecturer(sf::RenderWindow* hwnd, TextureManager* tm);

	//update method
	void update(float dt) override;

	sf::Text sizeTextToGrid(sf::Text t, float boardTop, float boardRight, float boardBottom, float boardLeft);
	sf::Text getMessageToDisplay(float boardTop, float boardRight, float boardBottom, float boardLeft);

	sf::RenderWindow* window;
	TextureManager* textMan;
	//messages displayed
	std::vector<std::string> motivationalMessages;
	//incremental value to show the next message
	int nextMessage;
	sf::Font font;
	//time the message is displayed for
	float msgDisplayTime;
	float msgDisplayTargetTime;
	//animation to make the lecturer look like they're speaking
	Animation speak;
	Animation* currentAnimation;
	//value to keep track of time alive
	float timeAlive;
	//speed of animation
	float animSpeed;

	int maxSize = 200;
};

