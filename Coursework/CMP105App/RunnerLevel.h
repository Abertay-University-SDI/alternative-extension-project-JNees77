#pragma once
#include "Framework/BaseLevel.h"
#include "TextureManager.h"
#include "Player.h"
#include <random>
#include <iostream>

class RunnerLevel : BaseLevel
{
public:	
	RunnerLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm);
	~RunnerLevel();

	void handleInput(float dt) override;
	void update(float dt) override;
	void render() override;
	//checks to see if the player collides with any blocks
	bool colliding(GameObject obj);
	//resets the characters speed values
	void reset();

private:
	TextureManager* textMan;
	//uses the player class
	Player p;

	std::vector<GameObject> BGs;
	std::vector<GameObject> jumpables;
	std::vector<GameObject> kickables;
	std::vector<GameObject> explosions;
	std::vector<float> explosionTimer;

	//progress bar stuff
	GameObject progressLine;
	Player progressP;
	sf::CircleShape destinationPoint;
	//finish line stuff
	GameObject moon;
	GameObject finishLine;
	//speed, distance, time
	float distance;
	float travelled = 0.f;
	float speed;

	const float MAX_SPEED = 650;
	const float ACCELERATION = 250;
	//end of game stuff
	int hits = 0;
	float time = 0.f;
	float objects = 0.f;
};

