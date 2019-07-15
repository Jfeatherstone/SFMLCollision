#include <SFML/Graphics.hpp>
#include "Polygon.hpp"

using namespace std;
using namespace sf;

/**
 * This example will show all four levels of detail for a given image.
 * Note that you may have to readjust the window if you decide to try another image.
 */
const string IMAGE_PATH = "Images/test.png";


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
        //    window.draw(p);

        window.display();
    }
    
    return 0;
}