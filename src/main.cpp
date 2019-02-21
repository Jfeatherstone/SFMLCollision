#include "Polygon.hpp"

Color colors[] = {Color::Red, Color::Yellow, Color::Green, Color::Blue, Color::Magenta};

int main() {

    Clock clock;

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    CircleShape c;
    c.setRadius(3);
    c.setOrigin(c.getGlobalBounds().width / 2, c.getGlobalBounds().height / 2);
    RectangleShape r;
    r.setSize(Vector2f(200, 50));

    clock.restart();
    Polygon poly(t, Detail::Optimal);
    poly.setScale(Vector2f(3, 3));
    poly.setFillColor(Color::Green);
    poly.setOrigin(poly.getCentroid());
    //cout << poly.getOrigin().x << " " << poly.getOrigin().y << endl;
    poly.setPosition(10, 30);
    
    Polygon poly2(t, Detail::More);
    poly2.setPosition(poly.getGlobalBounds().width + 40, 100);
    poly2.setScale(Vector2f(5, 5));
    poly2.setFillColor(Color::Magenta);
    poly2.setOrigin(poly2.getLocalBounds().width / 2, poly2.getLocalBounds().height / 2);
    
    Polygon poly3(t, Detail::Optimal);
    poly3.setPosition(2*(poly2.getGlobalBounds().width + 30) + 10, 30);
    //poly3.setPosition(100, 50);
    poly3.setScale(Vector2f(4, 4));
    poly3.setFillColor(Color::Red);
    
    // Benchmarking
    cout << clock.restart().asMilliseconds() << " milliseconds to create 3 polygons from a texture" << endl;
    
    poly2.setOrigin(poly2.getCentroid());

    /*
    // VERTEX DEBUGGING
    cout << "Less: " << poly.getPointCount() << endl;
    cout << "More: " << poly2.getPointCount() << endl;
    cout << "Perfect: " << poly3.getPointCount() << endl;
    */
    
    ///*
    // LINE DEBUGGING

    // Testing line intersection
    Line l1(Vector2f(0, 0), Vector2f(100, 0));
    l1.offset(Vector2f(80, 120));
    Line l2(Vector2f(0, 0), Vector2f(0, 100));
    l2.offset(Vector2f(100, 100));
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
    //*/

    /*
    // ROTATION TESTING
    Vector2f p(0, .5);
    float angle = 90;
    Vector2f origin(0, 0);
    cout << endl << "Rotation  Testing:" << endl;
    cout << "The point (" << p.x << ", " << p.y << ") rotated " << angle << " degrees about (" << origin.x << ", " << origin.y << ") is ";
    VectorMath::rotate(p, origin, angle);
    cout << "(" << p.x << ", " << p.y << ")" << endl;
    */

    // SETUP THE WINDOW
    RenderWindow window;
    window.create(VideoMode(poly.getGlobalBounds().width * 12 + 150, poly3.getGlobalBounds().height * 2 + 50), "Polygon Test", Style::Default);
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


        //cout << poly.getRotation() << endl;
        //poly.rotate(1000);
        //poly2.setScale(poly2.getScale() + Vector2f(.1, .1));
        //cout << poly.getOrigin().x << " " << poly.getOrigin().y << endl;
        //cout << poly.getGlobalBounds().left << ", " << poly.getGlobalBounds().top << endl;

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        // CLEAR
        window.clear();
        
        /*
        // INTERSECTION TESTING
        window.draw(poly);
        window.draw(poly2); 
        */
        window.setTitle(poly.intersects(poly2) ? "Colliding" : "Not colliding!");
        //Vector2f point;
        //window.setTitle(poly.getLines()[1].intersects(poly2.getLines()[1], point) ? "Colliding" : "Not colliding!");
        //c.setPosition(point);

        // LINE ALIGNMENT TESTING
        
        //window.draw(poly2);
        //window.draw(poly);
        //window.draw(poly3);
        ///*
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
        /*
        i = 0;
        for (Line l: poly3.getLines()) {
            window.draw(*l.getDrawable(colors[i]));
            i++;
            if (i > 4)
                i = 0;
        }
        */
        //window.draw(*poly.getLines()[1].getDrawable());
        //window.draw(*poly2.getLines()[1].getDrawable());
        //window.draw(c);

        // SHOW 
        window.display();
    }
    
    return 0;
}