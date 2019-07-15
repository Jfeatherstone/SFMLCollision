#include "Polygon.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace std;
using namespace sf;

int main() {

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    // Create all four polygons for each level of detail
    Polygon polygons[4] = {Polygon(t, Detail::Less),
                         Polygon(t, Detail::Optimal),
                         Polygon(t, Detail::More),
                         Polygon(t, Detail::Exact)};

    for (int i = 0; i < 4; i++) {
        polygons[i].setOrigin(polygons[i].getCentroid());
        polygons[i].setScale(5, 5);
        polygons[i].setPosition(100 + 200*i, 130);
    }

    // Print out the number of vertices on each level of detail
    cout << "Less: " << polygons[0].getPointCount() << endl;
    cout << "Optimal: " << polygons[1].getPointCount() << endl;
    cout << "More: " << polygons[2].getPointCount() << endl;
    cout << "Exact: " << polygons[3].getPointCount() << endl;

    // Setup the window
    RenderWindow window;
    window.create(VideoMode(800, 300), "Detail Comparison", Style::Default);
    window.setFramerateLimit(30);
        
    while (window.isOpen()) {

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear();
        
        // We could also just draw each shape here, but drawing the lines makes it a bit easier to
        // see the difference between the levels of detail (see below)

        for (Polygon p: polygons) {
            for (Line l: p.getLines())
                window.draw(*l.getDrawable());
        }

        // If you prefer the actual shapes
        //for (Polygon p: polygons)
        //    window.draw(p);

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