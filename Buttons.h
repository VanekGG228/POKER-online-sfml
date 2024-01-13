#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;


class Buttons
{
public:
	bool press = false;
	Buttons(Vector2f pos, Vector2f sizes,String Name) {
		position = pos;
		but.setPosition(pos);
		size = sizes;
		but.setSize(sizes);
		but.setFillColor(sf::Color(152, 104, 205));
		but.setOutlineThickness(3.f);
		but.setOutlineColor(Color(166, 150, 153));
		font.loadFromFile("font/arial_bold.ttf");
		text.setFont(font);
		text.setString(Name);
	}

	void ButtonPressColor();

	void draw(RenderWindow& app);
	bool wasPressed(RenderWindow& app);

private:
	RectangleShape but;
	Text text;
	Vector2f position;
	Vector2f size;
	Font font;
	Mouse::Button MouseClickButton; 

	void setPosText();


};

