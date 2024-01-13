#include "Card.h"

void Card::draw(RenderWindow& app)
{
    app.draw(card);
    app.draw(text);
    app.draw(spr[suit]);
}


void Card::setSize(Vector2f u, Sprite& spr) {
    Vector2f targetSize(u);
    spr.setScale(
        targetSize.x / spr.getLocalBounds().width,
        targetSize.y / spr.getLocalBounds().height);
}



void Card::CreateCard(int no, int sui) {
    nom = no;
    suit = sui;
    spr[suit].setPosition(card.getPosition().x + card.getSize().x / 2 - 30, card.getPosition().y + card.getSize().y / 2 - 30);
    PosText(nom, suit);
}

String Card:: resCard(int nom) {
    if (nom == 1) return 'A';
    else
        if (nom == 11) return 'J';
        else
            if (nom == 12) return 'Q';
            else
                if (nom == 13) return 'K';
                else
                    if (nom == 10)return "10";
    return char(nom + '0');
}

void Card::PosText(int nom, int suit) {
    font.loadFromFile("font/arial.ttf");
    text.setFont(font);
    text.setString(resCard(nom));
    text.setCharacterSize(40);
    if (suit <= 1)
        text.setFillColor(Color(0, 0, 0));
    else
        text.setFillColor(sf::Color::Red);
    text.setPosition(card.getPosition().x + 10, card.getPosition().y + 10);
}