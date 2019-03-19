#include "Polygon.hpp"

Color colors[] = {Color::Red, Color::Yellow, Color::Green, Color::Blue, Color::Magenta};

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test3.png");
    
    CircleShape c;
    c.setRadius(10);
    c.setOrigin(c.getGlobalBounds().width / 2, c.getGlobalBounds().height / 2);
    RectangleShape r;
    r.setSize(Vector2f(200, 0));

    clock.restart();
    //Polygon poly(t, Detail::Less);
    Polygon poly(c);
    poly.setScale(Vector2f(1, 1));
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getCentroid());
    poly.setPosition(10, 80);
    poly.setVelocity(Vector2f(5000, 0));

    Polygon poly2(t, Detail::Exact);
    //poly2.setPosition(poly.getGlobalBounds().width + 40, 30);
    poly2.setPosition(350, -20);
    poly2.setScale(Vector2f(5, 5));
    poly2.setFillColor(Color::Magenta);
    poly2.setAngularVelocity(10);

    Polygon poly3(t, Detail::More);
    poly3.setPosition(2*(poly2.getGlobalBounds().width + 30) + 10, 30);
    poly3.setScale(Vector2f(5, 5));
    poly3.setFillColor(Color::Red);
    
    // Benchmarking
    cout << clock.restart().asMilliseconds() << " milliseconds to create 3 polygons from a texture" << endl;
    
    /*
    // VERTEX DEBUGGING
    cout << "Less: " << poly.getPointCount() << endl;
    cout << "More: " << poly2.getPointCount() << endl;
    cout << "Perfect: " << poly3.getPointCount() << endl;
    */
    
    /*
    // LINE DEBUGGING

    // Testing line intersection
    Line l1(Vector2f(0, 0), Vector2f(100, 0));
    l1.offset(Vector2f(80, 120));
    Line l2(Vector2f(0, 0), Vector2f(0, 100));
    l2.offset(Vector2f(100, 100));
    cout << "L1 angle: " << l1.getAngle() / (M_PI / 180.0f) << endl;
    */

    // SETUP THE WINDOW
    RenderWindow window;
    window.create(VideoMode(poly2.getGlobalBounds().width * 3 + 150, poly3.getGlobalBounds().height + 50), "Polygon Test", Style::Default);
    window.setFramerateLimit(60);

    Clock time;

    while (window.isOpen()) {
        Time dt = time.restart();

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
            poly.rotate(1);
        }
        if (Keyboard::isKeyPressed(Keyboard::E)) {
            poly.rotate(-1);
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            poly.setScale(poly.getScale() + Vector2f(.03, .03));
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            poly.setScale(poly.getScale() - Vector2f(.03, .03));
        }

        //cout << time.getElapsedTime().asSeconds() << endl;
        poly.update(time.getElapsedTime().asSeconds());
        poly2.update(time.restart().asSeconds());

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear(); // CLEAR
        
        ///*
        // INTERSECTION TESTING
        window.draw(poly);
        window.draw(poly2);
        //window.draw(poly3);
        Vector2f res;
        window.setTitle(poly.intersects(poly2, res) ? "Colliding" : "Not colliding!");
        //*/

        /*
        // LINE ALIGNMENT TESTING
        
        window.draw(poly);
        window.draw(poly2);
        window.draw(poly3);
        int i = 0;
        for (Line l: poly.getLines()) {
            window.draw(*l.getDrawable(colors[i]));
            i++;
            if (i > 4)
                i = 0;
        }
        
        i = 0;
        for (Line l: poly2.getLines()) {
            window.draw(*l.getDrawable(colors[i]));
            i++;
            if (i > 4)
                i = 0;
        }
        
        i = 0;
        for (Line l: poly3.getLines()) {
            window.draw(*l.getDrawable(colors[i]));
            i++;
            if (i > 4)
                i = 0;
        }
        */

        // SHOW 
        window.display();
    }
    
    return 0;
}