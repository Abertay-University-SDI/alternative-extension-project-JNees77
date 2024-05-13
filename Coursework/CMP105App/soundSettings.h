#pragma once
#include "Framework/GameObject.h"
#include "Framework/GameState.h"
#include "Framework/BaseLevel.h"
#include "Framework/AudioManager.h"
#include "Framework/Collision.h"
#include "TextureManager.h"
#include "Cursor.h"
#include "Button.h"

class soundSettings:public BaseLevel
{
public:

	soundSettings();
	soundSettings(sf::RenderWindow* hwnd, Input* in, GameState* gs, AudioManager* aud, TextureManager* tm);
	~soundSettings();

	void update (float dt) override;
	void handleInput(float dt) override;
	void render();

	TextureManager* textMan;

	Button bar[2];
	Button dial[2];

	Cursor cursor;

	sf::Font font;
	sf::Text text[2];

	float minVol, volume, minPitch, pitch;
};

