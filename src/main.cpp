#include "Polygon.hpp"

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    clock.restart();
    Polygon poly(t, Detail::Less);
    poly.setScale(8, 8);
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getLocalBounds().width / 2, poly.getLocalBounds().height / 2);
    poly.setPosition(10, 30);
    Polygon poly2(t, Detail::More);
    poly2.setPosition(poly.getGlobalBounds().width + 40, 30);
    poly2.scale(8, 8);
    poly2.setFillColor(Color::Magenta);
    Polygon poly3(t, Detail::Optimal);
    poly3.setPosition(2*(poly2.getGlobalBounds().width + 30) + 10, 30);
    poly3.setScale(8, 8);
    poly3.setFillColor(Color::Red);
    cout << clock.restart().asMilliseconds() << " milliseconds to create 3 polygons from a texture" << endl;
    
    clock.restart();
    cout << endl << poly.intersects(poly2) << endl;
    cout << "Collision detection performed in " << clock.restart().asMilliseconds() << " milliseconds!" << endl;

    /*
    cout << endl << "Points (" << poly3.getPointCount() << "):" << endl;
    int i = 0;
    for (Vector2f p: poly3.getPoints()) {
        cout << i++ << " " << p.x << " " << p.y << endl;
    }
    cout << endl; */

    cout << "Less: " << poly.getPointCount() << endl;
    cout << "More: " << poly2.getPointCount() << endl;
    cout << "Perfect: " << poly3.getPointCount() << endl;

    RenderWindow window;
    window.create(VideoMode(poly3.getGlobalBounds().width * 3 + 150, poly3.getGlobalBounds().height + 50), "Polygon Test", Style::Default);
    window.setFramerateLimit(60);

    while (window.isOpen()) {

        ///////////////////////////////////////
        //          INPUT
        ///////////////////////////////////////

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            poly.setPosition(poly.getPosition() + Vector2f(5, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            poly.setPosition(poly.getPosition() + Vector2f(-5, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            poly.setPosition(poly.getPosition() + Vector2f(0, -5));
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            poly.setPosition(poly.getPosition() + Vector2f(0, 5));
        }
        if (Keyboard::isKeyPressed(Keyboard::Q)) {
            poly.rotate(5);
        }
        if (Keyboard::isKeyPressed(Keyboard::E)) {
            poly.rotate(-5);
        }

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear();

        window.draw(poly);
        window.draw(poly2); 
        window.draw(poly3);
        
        window.setTitle(poly.intersects(poly2) ? "Colliding" : "Not colliding!");

        window.display();
    }

   return 0;
}