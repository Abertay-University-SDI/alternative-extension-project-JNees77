#pragma once

#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Framework/BaseLevel.h"
#include "Framework/Collision.h"
#include "Button.h"
#include "Cursor.h"

class TitleScreen : BaseLevel {
public:
	TitleScreen(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm);
	~TitleScreen();

	void handleInput(float dt) override;
	void update(float dt);
	void render();

	void ButtonSetup();

	// Default functions for rendering to the screen.

	// Default variables for level class.
	TextureManager* textMan;

	GameObject fan;			// sadBear
	GameObject bg;
	sf::Text explanationText;	
	sf::Text titleText;
	sf::Font montsFont;

	sf::Text text[4];
	Button buttons[4];
	Cursor cursor;

	float animTimer;
	float x;

};