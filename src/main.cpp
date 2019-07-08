#include "Polygon.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>

// This main method creates a polygon test
/*
Color colors[] = {Color::Red, Color::Yellow, Color::Green, Color::Blue, Color::Magenta};

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    CircleShape c;
    c.setRadius(50);

    Polygon poly(t, Detail::Optimal);
    //Polygon poly(c);
    poly.setScale(4, 4);
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getCentroid());
    poly.setPosition(100, 120);

    Polygon poly2(t, Detail::Exact);
    //poly2.setPosition(poly.getGlobalBounds().width + 40, 30);
    poly2.setPosition(200, 50);
    poly2.setScale(Vector2f(5, 5));
    poly2.setFillColor(Color::Magenta);
    
    /*
    // VERTEX DEBUGGING
    cout << "Less: " << poly.getPointCount() << endl;test
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
    ///

    // SETUP THE WINDOW
    RenderWindow window;
    window.create(VideoMode(poly2.getGlobalBounds().width * 6 + 150, poly2.getGlobalBounds().height*5 + 50), "Polygon Test", Style::Default);
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

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear(); // CLEAR
        
        ///*
        // INTERSECTION TESTING
        window.draw(poly);
        window.draw(poly2);
        //window.draw(poly3);
        window.setTitle((poly2.intersects(poly)) ? "Colliding" : "Not colliding!");
        //

        ///*
        // LINE ALIGNMENT TESTING
        
        //window.draw(poly);
        //window.draw(poly2);
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
        //

        // SHOW 
        window.display();
    }
    
    return 0;
}
*/


// This main method creates a ball pit style simulation
/*
int main() {

    sf::Clock timeSinceCircle;
    sf::Clock clock;

    sf::Texture* binTex = new sf::Texture();
    binTex->loadFromFile("Images/bin.png");

    Polygon bin(binTex, Detail::Exact);
    bin.setOrigin(bin.getCentroid());
    bin.setPosition(250, 150);
    bin.setFillColor(sf::Color::Blue);
    bin.setScale(14, 16);
    bin.setMovableByCollision(false);
    bin.setRigidity(.75f);

    std::cout << bin.getPointCount() << std::endl;

    // SETUP THE WINDOW
    sf::RenderWindow window;
    window.create(sf::VideoMode(500, 400), "Ball Simulation", sf::Style::Default);
    window.setFramerateLimit(60);

    // Array of current objects to check collisions
    std::vector<Polygon> objects;

    //RectangleShape c;
    //c.setSize(Vector2f(30, 30));
    sf::CircleShape c(20);

    sf::Vector2f gravity(0, 1000);

    while (window.isOpen()) {
        float elapsed = clock.restart().asSeconds();
        sf::Vector2f res;
        
        for (int i = 0; i < objects.size(); i++)
            objects[i].update(elapsed);

        for (int i = 0; i < objects.size(); i++) {

            // Apply gravity
            bool colliding = false;

            colliding = objects[i].intersects(bin, res);
                

            for (int j = 0; j < objects.size(); j++) {
                if (j != i) {
                    if (objects[i].intersects(objects[j], res))
                        colliding = true;
                }
            }

            if (!colliding)
                objects[i].setVelocity(objects[i].getVelocity() + gravity * elapsed);

            //cout << objects[i].getAngularVelocity() << endl;
        }

        // Add balls at the mouse position on click
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && timeSinceCircle.getElapsedTime().asSeconds() > .1) {
            //cout << "Added ball!" << endl;
            Polygon ball(c);
            ball.setOrigin(ball.getCentroid());
            ball.setPosition( (sf::Vector2f)(sf::Mouse::getPosition() - window.getPosition()));
            if (!ball.intersects(bin)) {
                ball.setRigidity(1.0f);
                objects.push_back(ball);
                std::stringstream s;
                s << "Polyon Test (" << objects.size() << ")";
                window.setTitle(s.str());
                timeSinceCircle.restart();
            }
        }

        window.clear();

        for (Line l: bin.getLines()) {
            window.draw(*l.getDrawable());
        }

        for (Polygon p: objects) {
            for (Line l: p.getLines()) {
                window.draw(*l.getDrawable());
            }

            //window.draw(p);
        }
        //cout << objects.size() << endl;

        window.display();
    }

    return 0;
}
*/

using namespace sf;
using namespace std;

Color colors[] = {Color::Red, Color::Yellow, Color::Green, Color::Blue, Color::Magenta};

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    CircleShape c;
    c.setRadius(50);

    Polygon poly(t, Detail::Optimal);
    //Polygon poly(c);
    poly.setScale(4, 4);
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getCentroid());
    poly.setPosition(100, 120);

    Polygon poly2(t, Detail::Less);
    //poly2.setPosition(poly.getGlobalBounds().width + 40, 30);
    poly2.setPosition(200, 50);
    poly2.setScale(Vector2f(5, 5));
    poly2.setFillColor(Color::Magenta);
    
    /*
    // VERTEX DEBUGGING
    cout << "Less: " << poly.getPointCount() << endl;
    cout << "More: " << poly2.getPointCount() << endl;
    cout << "Perfect: " << poly3.getPointCount() << endl;
    //*/
    
    /*
    // LINE DEBUGGING

    // Testing line intersection
    Line l1(Vector2f(0, 0), Vector2f(100, 0));
    l1.offset(Vector2f(80, 120));
    Line l2(Vector2f(0, 0), Vector2f(0, 100));
    l2.offset(Vector2f(100, 100));
    cout << "L1 angle: " << l1.getAngle() / (M_PI / 180.0f) << endl;
    //*/

    // SETUP THE WINDOW
    RenderWindow window;
    window.create(VideoMode(poly2.getGlobalBounds().width * 6 + 150, poly2.getGlobalBounds().height*5 + 50), "Polygon Test", Style::Default);
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

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear(); // CLEAR
        
        ///*
        // INTERSECTION TESTING
        window.draw(poly);
        window.draw(poly2);
        //window.draw(poly3);
        window.setTitle((poly2.intersects(poly)) ? "Colliding" : "Not colliding!");
        //

        ///*
        // LINE ALIGNMENT TESTING
        
        //window.draw(poly);
        //window.draw(poly2);
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
        //

        // SHOW 
        window.display();
    }
    

    return 0;
}