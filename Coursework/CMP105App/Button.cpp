#include "Button.h"

Button::Button()
{

}
Button::Button(sf::RenderWindow* hwnd, Input* in, sf::Vector2f Size, sf::Vector2f Position)
{
	window = hwnd;
	input = in;

	setSize(Size);
	setPosition(Position);

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
void Button::setPositioning(sf::Vector2f coord)
{
	
}
void Button::render(sf::RenderWindow* hwnd)
{
	hwnd->draw(colBox);
}
