#include "Polygon.hpp"

int main() {

    CircleShape s;
    s.setRadius(100);
    RectangleShape r;
    r.setSize(Vector2f(100, 50));
    r.setPosition(250, 0);

    Texture* t = new Texture();
    t->loadFromFile("Images/test3.png");
    
    Polygon poly(t, Detail::Less);
    
    cout << endl << "Points:" << endl;
    for (Vector2f p: poly.getPoints()) {
        cout << p.x << " " << p.y << endl;
    } 
    cout << endl;

    RenderWindow window;
    s.setPosition(250, 0);
    window.create(VideoMode(900, 600), "Polygon Test", Style::Default);

    poly.setFillColor(Color::Blue);
    s.setFillColor(Color::Red);

    while (window.isOpen()) {
        window.clear();

        window.draw(poly);
        window.draw(r);

        window.display();
    }
    

   return 0;
} 