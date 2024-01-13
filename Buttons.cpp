#include "Buttons.h"


bool Buttons:: wasPressed(RenderWindow& app) {
	Vector2i MouseCoords = Mouse::getPosition(app);
	if (but.getGlobalBounds().contains(MouseCoords.x, MouseCoords.y) && Mouse::isButtonPressed(Mouse::Left)) {
		press = true;
		return true;
	}
	return false;
}
void Buttons::setPosText() {
	text.setCharacterSize(size.y / 2);
	text.setFillColor(Color::White);
	text.setPosition(position.x + text.getCharacterSize() / 2, position.y + text.getCharacterSize() / 2);
}

void Buttons::ButtonPressColor() {
	but.setFillColor(sf::Color(214, 163, 148));
}


void Buttons::draw(RenderWindow& app)
{
	setPosText();
	app.draw(but);
	app.draw(text);
	Vector2i MouseCoords = Mouse::getPosition(app);
	Vector2u WinSize = (app).getSize();
	text.setFillColor(Color::White);
}