#include "Button.h"

Button::Button()
{

}
//overloaded constructor setting up the properties of the button
Button::Button(sf::RenderWindow* hwnd, Input* in, sf::Vector2f Size, sf::Vector2f Position)
{
	window = hwnd;
	input = in;

	setSize(Size);
	setPosition(Position);
	//sets up the collision box object so it vissible on screen
	colBox.setPosition(getCollisionBox().left, getCollisionBox().top);
	colBox.setSize(sf::Vector2f(getCollisionBox().width, getCollisionBox().height));
	colBox.setFillColor(sf::Color::Magenta);
	colBox.setOutlineColor(sf::Color::White);
	colBox.setOutlineThickness(4.f);
}
Button::~Button()
{

}
void Button::update(float dt)
{
	//updates the position ans size on the collision box + the origion and sets the object actual collision box
	colBox.setPosition(getCollisionBox().left, getCollisionBox().top);
	colBox.setSize(sf::Vector2f(getCollisionBox().width, getCollisionBox().height));
	setOrigin(getSize().x / 2, getSize().y / 2);
	setCollisionBox(0, 0, getSize().x, getSize().y);
}
void Button::handleInput(float dt)
{

}
void Button::collisionResponse(GameObject* collider)
{

}
void Button::render(sf::RenderWindow* hwnd)
{
	//draws the visible collision box
	hwnd->draw(colBox);
}
