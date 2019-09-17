#include "Polygon.hpp"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;
using namespace std;

/**
 * This example will show all four levels of detail for a given image.
 * Note that you may have to readjust the window if you decide to try another image.
 */
const string IMAGE_PATH = "Images/daniels_particles_test_4.png";
//const string IMAGE_PATH = "Images/test.png";

int main() {

    Texture* t = new Texture();
    t->loadFromFile(IMAGE_PATH);
    
    // Create all four polygons for each level of detail
    Polygon polygons[4] = {Polygon(t, Detail::Less),
                         Polygon(t, Detail::Optimal),
                         Polygon(t, Detail::More),
                         Polygon(t, Detail::Exact)};

    for (int i = 0; i < 4; i++) {
        polygons[i].setOrigin(polygons[i].getCentroid());
        polygons[i].setSize(150, 150);
        polygons[i].setPosition(90 + 200*i, 130);
    }

    // Print out the number of vertices on each level of detail
    cout << "Less: " << polygons[0].getPointCount() << endl;
    cout << "Optimal: " << polygons[1].getPointCount() << endl;
    cout << "More: " << polygons[2].getPointCount() << endl;
    cout << "Exact: " << polygons[3].getPointCount() << endl;

    // Setup the window
    RenderWindow window;
    window.create(VideoMode(800, 300), "Detail Comparison", Style::Default);
    window.setFramerateLimit(60);
        
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
            //window.draw(p);

        window.display();
    }
    
    return 0;
}

/*
int main() {

    Texture* t = new Texture();
    t->loadFromFile("Images/test4.png");
    
    // Create both polygons (the level of detail is that important here)
    Polygon polygons[2] = {Polygon(t, Detail::More),
                         Polygon(t, Detail::More)};

    //delete t;

    //CircleShape c(10);
    //Polygon polygons[2] = {Polygon(c), Polygon(c)};

    for (int i = 0; i < 2; i++) {
        polygons[i].setOrigin(polygons[i].getCentroid());
        polygons[i].setScale(5, 5);
        polygons[i].setPosition(100 + 300*i, 130 + i*100);
    }

    polygons[0].setVelocity(sf::Vector2f(75, 0));
    //polygons[1].setVelocity(sf::Vector2f(-75, 0));
    polygons[0].setAngularVelocity(100);

    polygons[0].setDegreesOfFreedom(true, true);

    // Setup the window
    RenderWindow window;
    window.create(VideoMode(500, 400), "Collision test", Style::Default);
    window.setFramerateLimit(60);
        
    Clock time;
    
    bool paused = false;

    while (window.isOpen()) {
        
        Time dt = time.restart();

        ///*
        ///////////////////////////////////////
        //          INPUT
        ///////////////////////////////////////

        if (window.hasFocus()) {
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
        ///*

            Event event;
            while(window.pollEvent(event)) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                    paused = !paused;
                }
            }
        }

        //polygons[0].addForce(Force(sf::Vector2f(1000, 0), 1.0f, sf::Vector2f(0, -10)));
        //polygons[0].addForce(Force(sf::Vector2f(-.641, -.641), .10f, sf::Vector2f(0, -10)));

        //std::cout << polygons[0].getPosition().x << std::endl;

        if (!paused) {
            ///////////////////////////////////////
            //          DRAWING
            ///////////////////////////////////////
            window.clear();

            for (Polygon p: polygons) {
                for (Line l: p.getLines())
                    window.draw(*l.getDrawable());
                //window.draw(p);
            }

            // DEBUG
            // Add all of the normal lines to the list of drawing objects
            for (Line l: polygons[0].getLines()) {
                sf::Vector2f lineCenter = (l.getStart() + l.getEnd()) / 2.0f;
                window.draw(*Line(lineCenter, lineCenter - l.getNormal() * 30.0f).getDrawable(Color::Green));
            }

            for (sf::Shape* s: polygons[0].intersectAndResolve(polygons[1]))
                window.draw(*s);

            polygons[0].update(dt.asSeconds());
            polygons[1].update(dt.asSeconds());

            window.display();
        }
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
    bin.setPosition(250, 230);
    bin.setFillColor(sf::Color::Blue);
    bin.setScale(14, 22);
    bin.setDegreesOfFreedom(false, false);
    bin.setYoungsModulus(.75f);

    //bin.setVelocity(Vector2f(0, -1));
    //bin.setAngularVelocity(10);

    std::cout << bin.getPointCount() << std::endl;

    // SETUP THE WINDOW
    sf::RenderWindow window;
    window.create(sf::VideoMode(500, 450), "Ball Simulation", sf::Style::Default);
    window.setFramerateLimit(60);

    // Array of current objects to check collisions
    std::vector<Polygon> objects;

    //RectangleShape c;
    //c.setSize(Vector2f(50, 30));
    sf::CircleShape c(20);

    float gravity = 10000;

    while (window.isOpen()) {
        float elapsed = clock.restart().asSeconds();
        
        bin.update(elapsed);
        for (int i = 0; i < objects.size(); i++)
            objects[i].update(elapsed);

        for (int i = 0; i < objects.size(); i++) {

            // Apply gravity
            bool colliding = false;

            colliding = objects[i].intersectAndResolve(bin).size() > 0;
                
            
            for (int j = 0; j < objects.size(); j++) {
                if (j != i) {
                    if (objects[i].intersectAndResolve(objects[j]).size() > 0)
                        colliding = true;
                }
            }

            if (!colliding)
                objects[i].addForce(Force(sf::Vector2f(0, 1), gravity));

            //cout << objects[i].getAngularVelocity() << endl;
        }

        // Add balls at the mouse position on click
        if (window.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && timeSinceCircle.getElapsedTime().asSeconds() > .1) {
            //cout << "Added ball!" << endl;
            Polygon ball(c);
            ball.setOrigin(ball.getCentroid());
            ball.setPosition( (sf::Vector2f)(sf::Mouse::getPosition() - window.getPosition()));
            if (!ball.intersects(bin)) {
                ball.setYoungsModulus(.95f);
                objects.push_back(ball);
                std::stringstream s;
                s << "Polyon Test (" << objects.size() << ")";
                window.setTitle(s.str());
                timeSinceCircle.restart();
            }
        }

        window.clear();

        // DEBUG
        // Add all of the normal lines to the list of drawing objects
        for (Line l: bin.getLines()) {
            sf::Vector2f lineCenter = (l.getStart() + l.getEnd()) / 2.0f;
            window.draw(*Line(lineCenter, lineCenter - l.getNormal() * 30.0f).getDrawable(Color::Green));
        }

        for (Line l: bin.getLines()) {
            window.draw(*l.getDrawable());
        }

        for (Polygon p: objects) {
            for (Line l: p.getLines()) {
                window.draw(*l.getDrawable());

                sf::Vector2f lineCenter = (l.getStart() + l.getEnd()) / 2.0f;
                window.draw(*Line(lineCenter, lineCenter - l.getNormal() * 10.0f).getDrawable(Color::Green));

            }

            //window.draw(p);
        }
        //cout << objects.size() << endl;

        window.display();
    }

    return 0;
}
//*/

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