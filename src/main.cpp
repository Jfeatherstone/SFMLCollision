#include "Line.hpp"
#include "Polygon.hpp"

int main() {

    CircleShape s;
    s.setRadius(100);

    Texture* t;
    t->loadFromFile("Images/test.png");

    Polygon poly(t);
    for (Vector2f p: poly.getPoints()) {
        cout << p.x << " " << p.y << endl;
    } 

    RenderWindow window;
    s.setPosition(250, 0);
    window.create(VideoMode(900, 600), "Polygon Test", Style::Default);

    poly.setFillColor(Color::Blue);
    s.setFillColor(Color::Red);

    while (window.isOpen()) {
        window.clear();

        window.draw(poly);
        window.draw(s);

        window.display();
    }
} 