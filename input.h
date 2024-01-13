#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class input
{
public:

	RectangleShape shape;
	
	input(Vector2f position, Vector2f sizes){
		shape.setPosition(position);
		shape.setSize(sizes);
		shape.setFillColor(sf::Color(152, 104, 205));
		shape.setOutlineThickness(3.f);
		shape.setOutlineColor(Color(166, 150, 153));
	}
	void setSize(Vector2f size) {
		shape.setSize(size);
	}
	void draw(RenderWindow& app) {
		app.draw(shape); 
	}
private:
	 
};

