#include "Cursor.h"

Cursor::Cursor()
{

}
Cursor::Cursor(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;
	//size of the cursor object
	setSize(sf::Vector2f(10.f, 10.f));
	setOutlineThickness(2.f);
}
Cursor::~Cursor()
{

}
void Cursor::update(float dt)
{
	//updates the position of the cursor object using the cursor position
	x = input->getMouseX();
	y = input->getMouseY();
    setPosition(x, y);
	//sets up collision box
	setCollisionBox(0, 0, getSize().x, getSize().y);
}
void Cursor::handleInput(float dt)
{

}
void Cursor::collisionResponse(GameObject* collider)
{

}
