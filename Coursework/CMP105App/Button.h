#pragma once
#include "SFML/Graphics.hpp"
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Framework/Input.h"
#include <string.h>
#include <iostream>

class Button :public GameObject
{
public:
	Button();
	Button(sf::RenderWindow* hwnd, Input* in);
	~Button();

	void setPositioning(sf::Vector2f coord);
	void update(float dt) override;
	void handleInput(float dt) override;
	void render(sf::RenderWindow* hwnd);
	void collisionResponse(GameObject* collider) override;

private:

	GameObject colBox;
};

