#include "Polygon.hpp"

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    clock.restart();
    Polygon poly(t, Detail::Less);
    poly.setScale(Vector2f(3, 3));
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getLocalBounds().width / 2, poly.getLocalBounds().height / 2);
    poly.setPosition(10, 30);
    Polygon poly2(t, Detail::More);
    poly2.setPosition(poly.getGlobalBounds().width + 40, 30);
    poly2.setScale(Vector2f(3, 3));
    poly2.setFillColor(Color::Magenta);
    Polygon poly3(t, Detail::Optimal);
    poly3.setPosition(2*(poly2.getGlobalBounds().width + 30) + 10, 30);
    //poly3.setPosition(100, 50);
    poly3.setScale(Vector2f(4, 4));
    poly3.setFillColor(Color::Red);
    cout << clock.restart().asMilliseconds() << " milliseconds to create 3 polygons from a texture" << endl;
    
    /*
    // INTERSECTION BENCHMARKING
    
    clock.restart();
    cout << endl << poly.intersects(poly2) << endl;
    cout << "Collision detection performed in " << clock.restart().asMilliseconds() << " milliseconds!" << endl;
    */

    /*
    // VERTEX DEBUGGING

    cout << endl << "Points (" << poly3.getPointCount() << "):" << endl;
    int i = 0;
    for (Vector2f p: poly3.getPoints()) {
        cout << i++ << " " << p.x << " " << p.y << endl;
    }
    cout << endl;
    */

    cout << "Less: " << poly.getPointCount() << endl;
    cout << "More: " << poly2.getPointCount() << endl;
    cout << "Perfect: " << poly3.getPointCount() << endl;

    
    /*
    // LINE DEBUGGING

    // Testing line intersection
    Line l1(Vector2f(-1, -1), Vector2f(2, -1));
    Line l2(Vector2f(0, 0), Vector2f(1, 1));
    cout << "L1 angle: " << l1.getAngle() / (M_PI / 180.0f) << endl;

    Vector2f v;
    stringstream s;
    bool b = l1.intersects(l2, v);
    if (b)
        s << "Intersect at (" << v.x << ", " << v.y << ", * M_PI / 180 1)";
    else
        s << "Don't intersect";
    cout << endl << s.str() << endl << "(" << l1.getStart().x << ", " << l1.getStart().y << ") to (" << l1.getEnd().x << ", " << l1.getEnd().y << ")" << endl;
    cout << "(" << l2.getStart().x << ", " << l2.getStart().y << ") to (" << l2.getEnd().x << ", " << l2.getEnd().y << ")" << endl;
    */

    // SETUP THE WINDOW
    RenderWindow window;
    window.create(VideoMode(poly.getGlobalBounds().width * 3 + 150, poly3.getGlobalBounds().height + 50), "Polygon Test", Style::Default);
    window.setFramerateLimit(60);

    while (window.isOpen()) {

        ///////////////////////////////////////
        //          INPUT
        ///////////////////////////////////////

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            poly.setPosition(poly.getPosition() + Vector2f(1, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            poly.setPosition(poly.getPosition() + Vector2f(-1, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            poly.setPosition(poly.getPosition() + Vector2f(0, -1));
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            poly.setPosition(poly.getPosition() + Vector2f(0, 1));
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
        // CLEAR
        window.clear();
        
        /*
        // INTERSECTION TESTING
        window.draw(poly);
        window.draw(poly2); 
        window.setTitle(poly.intersects(poly2) ? "Colliding" : "Not colliding!");
        */

        // LINE ALIGNMENT TESTING;
        window.draw(poly3);
        for (Line l: poly3.getLines()) {
            window.draw(*l.getDrawable());
        }
        for (Line l: poly2.getLines()) {
            window.draw(*l.getDrawable());
        }


        // SHOW
        window.display();
    }
    
    return 0;
}