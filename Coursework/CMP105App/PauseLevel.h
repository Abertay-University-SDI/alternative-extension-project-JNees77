#pragma once
#include "SFML/Graphics.hpp"
#include "Framework/GameState.h"
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Framework/Collision.h"
#include "TextureManager.h"
#include "Framework/Input.h"
#include <Windows.h>
#include "Lecturer.h"
#include "Level.h"

#include "Button.h"
#include "Cursor.h"

class PauseLevel:public BaseLevel
{
public:
	PauseLevel();
	PauseLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, TextureManager* tm);
	~PauseLevel();

	void update(float dt) override;
	void handleInput(float dt) override;
	void render();

	GameObject fan;
	GameObject bg;

	TextureManager* textMan;

	Button button[2];
	sf::Font font;
	sf::Text PausedText;
	sf::Text text[2];

	Cursor cursor;

	GameState* savedState;

private:

	float x, y, animTimer;
};

