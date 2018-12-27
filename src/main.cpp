#include "Polygon.hpp"

int main() {

    RectangleShape r;
    r.setSize(Vector2f(100, 50));
    r.setPosition(250, 0);

    Texture* t = new Texture();
    t->loadFromFile("Images/test.png");
    
    Polygon poly(t, Detail::Less);
    poly.setScale(8, 8);
    poly.setFillColor(Color::Green);
   /* Polygon poly2(t, Detail::More);
    poly2.setPosition(0, 250);
    poly2.scale(8, 8);
    Polygon poly3(t, Detail::Perfect);
    poly3.setPosition(0, 500);
    poly3.setScale(8, 8);
    poly3.setFillColor(Color::Red);*/

    
   /*cout << endl << "Points (" << poly.getPointCount() << "):" << endl;
   int i = 0;
    for (Vector2f p: poly.getPoints()) {
        cout << i++ << " " << p.x << " " << p.y << endl;
    }
    cout << endl; */

    cout << "Less: " << poly.getPointCount() << endl;
    //cout << "More: " << poly2.getPointCount() << endl;
    //cout << "Perfect: " << poly3.getPointCount() << endl;

    RenderWindow window;
    window.create(VideoMode(1000, 800), "Polygon Test", Style::Default);
    
    while (window.isOpen()) {
        window.clear();

        window.draw(poly);
        //window.draw(poly2); 
        //window.draw(poly3);

        window.display();
    }
    

   return 0;
}