#pragma once
#include "Framework/GameObject.h"
#include "SFML/Graphics.hpp"
#include "Framework/Input.h"
#include <iostream>

class Cursor: public GameObject
{
public:
	Cursor();
	Cursor(sf::RenderWindow* hwnd, Input* in);
	~Cursor();

	void collisionResponse(GameObject* collider) override;
	void update(float dt) override;
	void handleInput(float dt) override;

	float x, y;
};

