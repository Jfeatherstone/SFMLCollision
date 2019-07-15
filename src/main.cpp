#include "Polygon.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>

// This main method creates a polygon test
///*

using namespace sf;
using namespace std;

Color colors[] = {Color::Red, Color::Yellow, Color::Green, Color::Blue, Color::Magenta};

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    CircleShape c;
    c.setRadius(50);

    Polygon poly(t, Detail::Less);
    //Polygon poly(c);
    poly.setScale(5, 5);
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getCentroid());
    poly.setPosition(100, 150);

    Polygon poly2(t, Detail::Optimal);
    poly2.setOrigin(poly2.getCentroid());
    poly2.setPosition(300, 150);
    poly2.setScale(Vector2f(5, 5));
    poly2.setFillColor(Color::Magenta);
    
    Polygon poly3(t, Detail::More);
    poly3.setOrigin(poly3.getCentroid());
    poly3.setPosition(500, 150);
    poly3.setScale(Vector2f(5, 5));
    poly3.setFillColor(Color::Magenta);

    Polygon poly4(t, Detail::Exact);
    poly4.setOrigin(poly4.getCentroid());
    poly4.setPosition(700, 150);
    poly4.setScale(Vector2f(5, 5));
    poly4.setFillColor(Color::Magenta);

    ///*
    // VERTEX DEBUGGING
    cout << "Less: " << poly.getPointCount() << endl;
    cout << "Optimal: " << poly2.getPointCount() << endl;
    cout << "More: " << poly3.getPointCount() << endl;
    cout << "Exact: " << poly4.getPointCount() << endl;
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
    window.create(VideoMode(900, 400), "Polygon Test", Style::Default);
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
        //window.draw(poly);
        //window.draw(poly2);
        //window.draw(poly3);
        std::vector<sf::Vector2u> intersectingLines = poly.intersects(poly2);
        window.setTitle((intersectingLines.size() > 0) ? "Colliding" : "Not colliding!");
        //

        ///*
        // LINE ALIGNMENT TESTING
        
        //window.draw(poly);
        //window.draw(poly2);
        for (Line l: poly.getLines()) {
            window.draw(*l.getDrawable(sf::Color::White));
        }
        
        for (Line l: poly2.getLines()) {
            window.draw(*l.getDrawable(sf::Color::White));
        }

        for (Line l: poly3.getLines()) {
            window.draw(*l.getDrawable(sf::Color::White));
        }

        for (Line l: poly4.getLines()) {
            window.draw(*l.getDrawable(sf::Color::White));
        }

        for (sf::Vector2u v: intersectingLines) {
            window.draw(*poly.getLines()[v.x].getDrawable(sf::Color::Red));
            window.draw(*poly2.getLines()[v.y].getDrawable(sf::Color::Red));
        }
        //*/

        // SHOW 
        window.display();
    }
    
    return 0;
}
//*/


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

/*
using namespace sf;
using namespace std;

Color colors[] = {Color::Red, Color::White, Color::Green, Color::Blue, Color::Magenta};

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    CircleShape c;
    c.setRadius(10);

    sf::Vector2f windowSize(800, 600);

    Polygon poly(t, Detail::Exact);
    //Polygon poly(c);
    poly.setScale(8, 8);
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getCentroid());
    poly.setPosition(windowSize.x / 2, windowSize.y / 2);

    // Show each point on the shape
    sf::CircleShape vertices[poly.getPointCount()];

    // SETUP THE WINDOW
    RenderWindow window;
    window.create(VideoMode(windowSize.x, windowSize.y), "Polygon Test", Style::Default);
    window.setFramerateLimit(60);
    
    for (Line l: poly.getLines()) {
        std::cout << l.getStart().x << " " << l.getStart().y << " - " << l.getEnd().x << " " << l.getEnd().y << std::endl;
    }

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

        // Update the vertices
        for (int i = 0; i < poly.getPointCount(); i++) {
            vertices[i].setRadius(3);
            vertices[i].setFillColor(colors[i % 5]);
            vertices[i].setOrigin(vertices[i].getLocalBounds().width / 2, vertices[i].getLocalBounds().height / 2);
            vertices[i].setPosition(poly.getPosition() - 
                (sf::Vector2f(poly.getOrigin().x * poly.getScale().x, poly.getOrigin().y * poly.getScale().y)) +
                (sf::Vector2f(poly.getPoint(i).x * poly.getScale().x, poly.getPoint(i).y * poly.getScale().y)));
        }

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear(); // CLEAR
        
        //window.draw(poly);

        ///*        
        int i = 0;
        for (Line l: poly.getLines()) {
            window.draw(*l.getDrawable(colors[i]));
            i++;
            if (i > 4)
                i = 0;
        }
        //*

        for (sf::CircleShape s: vertices)
            window.draw(s);

        //window.draw(*poly.getLines()[0].getDrawable());
        //window.draw(*poly.getLines()[1].getDrawable());
        //std::cout << poly.getLines()[0].getStart().x << " " << poly.getLines()[0].getStart().y << " - " << poly.getLines()[0].getEnd().x << " " << poly.getLines()[0].getEnd().y << endl;


        // SHOW 
        window.display();
    }
    

    return 0;
}
*/