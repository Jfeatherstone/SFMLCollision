#include "Polygon.hpp"

int main() {

    Clock clock;

    RectangleShape r;
    r.setSize(Vector2f(100, 50));
    r.setPosition(250, 0);

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    clock.restart();
    Polygon poly(t, Detail::Less);
    poly.setScale(8, 8);
    poly.setFillColor(Color::Green);
    poly.setPosition(10, 0);
    Polygon poly2(t, Detail::More);
    poly2.setPosition(poly.getGlobalBounds().width + 40, 0);
    poly2.scale(8, 8);
    Polygon poly3(t, Detail::Perfect);
    poly3.setPosition(2*(poly.getGlobalBounds().width + 30) + 10, 0);
    poly3.setScale(8, 8);
    poly3.setFillColor(Color::Red);
    cout << clock.restart().asMilliseconds() << " milliseconds to create 3 polygons from a texture" << endl;
    
    /*
    This is for overlaying that actual image and seeing how it looks
    */
    Sprite s;
    s.setTexture(*t);
    s.setPosition(2*(poly.getGlobalBounds().width + 30) + 10, 0);
    s.setScale(8, 8);
    s.setColor(Color(0, 0, 0, 150));
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
    
    while (window.isOpen()) {
        window.clear();

        window.draw(poly);
        window.draw(poly2); 
        window.draw(poly3);
        //window.draw(s);
        
        window.display();
    }
    

   return 0;
}