#include "Polygon.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace std;
using namespace sf;

int main() {

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    // Create both polygons (the level of detail is that important here)
    Polygon polygons[2] = {Polygon(t, Detail::More),
                         Polygon(t, Detail::More)};

    for (int i = 0; i < 2; i++) {
        polygons[i].setOrigin(polygons[i].getCentroid());
        polygons[i].setScale(5, 5);
        polygons[i].setPosition(100 + 200*i, 130 + i*90);
    }

    polygons[0].setVelocity(sf::Vector2f(30, 0));

    // Setup the window
    RenderWindow window;
    window.create(VideoMode(500, 400), "Collision test", Style::Default);
    window.setFramerateLimit(60);
        
    Clock time;
    
    while (window.isOpen()) {
        
        Time dt = time.restart();

        /*
        ///////////////////////////////////////
        //          INPUT
        ///////////////////////////////////////

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            polygons[0].setPosition(polygons[0].getPosition() + Vector2f(1, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            polygons[0].setPosition(polygons[0].getPosition() + Vector2f(-1, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            polygons[0].setPosition(polygons[0].getPosition() + Vector2f(0, -1));
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            polygons[0].setPosition(polygons[0].getPosition() + Vector2f(0, 1));
        }
        if (Keyboard::isKeyPressed(Keyboard::Q)) {
            polygons[0].rotate(1);
        }
        if (Keyboard::isKeyPressed(Keyboard::E)) {
            polygons[0].rotate(-1);
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            polygons[0].setScale(polygons[0].getScale() + Vector2f(.03, .03));
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            polygons[0].setScale(polygons[0].getScale() - Vector2f(.03, .03));
        }
        */

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear();

        polygons[0].intersectAndResolve(polygons[1]);

        polygons[0].update(dt.asSeconds());
        polygons[1].update(dt.asSeconds());

        for (Polygon p: polygons) {
            for (Line l: p.getLines())
                window.draw(*l.getDrawable());
        }

        window.display();
    }
    
    return 0;
}

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