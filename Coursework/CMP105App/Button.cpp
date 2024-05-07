#include "Button.h"

Button::Button()
{

}
Button::Button(sf::RenderWindow* hwnd, Input* in)
{
	window = hwnd;
	input = in;

	setSize(sf::Vector2f(400, 100));
	setPosition(100,100);

	colBox.setFillColor(sf::Color::Magenta);
	colBox.setOutlineColor(sf::Color::White);
	colBox.setOutlineThickness(4.f);
}
Button::~Button()
{

}
void Button::update(float dt)
{
	setCollisionBox(0, 0, getSize().x, getSize().y);
	colBox.setPosition(getCollisionBox().left, getCollisionBox().top);
	colBox.setSize(sf::Vector2f(getCollisionBox().width, getCollisionBox().height));
}
void Button::handleInput(float dt)
{

}
void Button::collisionResponse(GameObject* collider)
{

}
void Button::setPositioning(sf::Vector2f coord)
{
	setPosition(coord);
	setOrigin(getSize().x / 2, getSize().y / 2);
}
void Button::render(sf::RenderWindow* hwnd)
{
	hwnd->draw(colBox);
}
