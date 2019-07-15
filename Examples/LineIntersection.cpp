#include <SFML/Graphics.hpp>
#include "Polygon.hpp"

/**
 * This example allows the user to move one shape around using the arrow keys, as well
 * as scale/rotate it. If any of the lines are colliding with the other shape, they
 * will be highlighted on both shapes.
 * 
 * This example will likely not work into release, see the while loop for why
 */

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
        polygons[i].setPosition(100 + 200*i, 130);
    }

    // Setup the window
    RenderWindow window;
    window.create(VideoMode(400, 300), "Collision test", Style::Default);
    window.setFramerateLimit(60);
        
    while (window.isOpen()) {
        
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

        ///////////////////////////////////////
        //          DRAWING
        ///////////////////////////////////////
        window.clear();

        // At this point, the intersects method only returns this vector of points
        // as a debuggin tool. This will later be changed to simply a bool, and
        // so this example may not longer work because of that.
        vector<Vector2u> intersectingLines = polygons[0].intersects(polygons[1]);

        for (Polygon p: polygons) {
            for (Line l: p.getLines())
                window.draw(*l.getDrawable());
        }

        for (Vector2u v: intersectingLines) {
            window.draw(*polygons[0].getLines()[v.x].getDrawable(Color::Red));
            window.draw(*polygons[1].getLines()[v.y].getDrawable(Color::Blue));
        }

        window.display();
    }
    
    return 0;
}