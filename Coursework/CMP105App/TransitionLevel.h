#pragma once
#include "Framework/BaseLevel.h"
#include "Framework/GameObject.h"
#include "Framework/Collision.h"
#include "TextureManager.h"
#include "Cursor.h"
#include "Button.h"
#include <Windows.h>

class TransitionLevel : public BaseLevel
{
public:
	TransitionLevel(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm);
	~TransitionLevel();

	void handleInput(float dt) override;
	void update(float dt);
	void render();

private:
	TextureManager* textMan;
	sf::Font font;
	sf::Text explain;
	GameObject bg;

	Cursor cursor;
	Button returnButton;
	sf::Text buttonTxt;


};

