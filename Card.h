#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Card
{
public:
   
    Card() {
        card.setSize(Vector2f(150, 220));
        card.setFillColor(sf::Color(255, 255, 255));
        card.setOutlineThickness(3.f);
        card.setOutlineColor(sf::Color(0, 0, 0));
        h.loadFromFile("image/hearts.png");
        p.loadFromFile("image/peak.png");
        k.loadFromFile("image/krest.png");
        b.loadFromFile("image/boob.png");
        spr[0].setTexture(k);
        spr[1].setTexture(p);
        spr[2].setTexture(h);
        spr[3].setTexture(b);
        for (int i = 0; i < 4; i++) {
            setSize(Vector2f(60.f, 60.f),spr[i]);
        }
    };
    void draw(RenderWindow& app);

    Vector2f gtSize() {
        return card.getSize();
    }

    void stPos(Vector2f x) {
        card.setPosition(x);
    }

    void setSize(Vector2f u, Sprite& spr);

    void CreateCard(int nom, int suit);

private:
    int nom;
    int suit;
    Vector2f m_size;
    Sprite spr[4];
    RectangleShape card;
    Text text;
    Texture h;
    Texture p;
    Texture k;
    Texture b;
    Font font;
    

    String resCard(int nom);

    void PosText(int nom, int suit);
    

};

