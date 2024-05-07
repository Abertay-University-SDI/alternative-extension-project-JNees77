#include "Cursor.h"

Cursor::Cursor()
{

}
Cursor::Cursor(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	setSize(sf::Vector2f(10.f, 10.f));
	setOutlineThickness(2.f);
}
Cursor::~Cursor()
{

}
void Cursor::update(float dt)
{
	//float x = sf::Mouse::getPosition().x;
	//float y = sf::Mouse::getPosition().y;
	x = input->getMouseX();
	y = input->getMouseY();

	setPosition(x, y);
	setCollisionBox(0, 0, getSize().x, getSize().y);
}
void Cursor::handleInput(float dt)
{

}
void Cursor::collisionResponse(GameObject* collider)
{

}
