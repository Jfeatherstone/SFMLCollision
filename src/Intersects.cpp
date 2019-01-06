#include "Polygon.hpp"

/*
    TODO:
If the first 3 methods are called, a polygon with window-based coordinates will be passed
into the last one, which won't happen if the last one is just called on its own. Figure out
a way to make sure that either method will result in the correct decision being made
*/

bool Polygon::intersects(RectangleShape shape) {
    // Since there is a getPoint() method for this, we don't have to do any math, just add the points to a vector
    vector<Vector2f> points;

    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    Polygon poly(points);
    return intersects(poly);
    
}

/*
For these next two, it is important to note that we don't have to do any math on the verticies because
they are given in local coordinates, in relation to 0, 0 being at the top left of the actual shape's rectangle
*/

bool Polygon::intersects(CircleShape shape) {
    // Since there is a getPoint() method for this, we don't have to do any math, just add the points to a vector
    vector<Vector2f> points;

    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    Polygon poly(points);
    return intersects(poly);
    
}

/*
Because of the similarity between the convexshape and circleshape classes (having getPoint and getPointCount) we
can have the exact same code work for both
*/
bool Polygon::intersects(ConvexShape shape) {
    vector<Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }
    
    Polygon poly(points);
    return intersects(poly);
}

/*
    The big one

This is the actual intersection method, that is called in all of the above "wrappers" for it

It uses barycentric coordinates as well as proximity detection to reduce the amount of calculations
when applicable

Also, up until this point we have been ignoring a pretty large issue about the nature of this class:
we have only worried about the local coordinates of the verticies. That is, we now will need to add certain
values to our vertex coordinates to find their position on the screen -- and in relation to another object on the
screen -- by using the getPosition() function defined in Shape. We assume here that the user has been updating
the polygons' location with the setPosition() properly
*/

bool Polygon::intersects(Polygon shape) {
    
    /*
    We first check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    */
    if (!getGlobalBounds().intersects(shape.getGlobalBounds())) {
        //cout << "Rect bounds" << endl;
        return false;
    }
    /*
    The next order of business here is that we need to adjust our lines to be relative to the window they are
    drawn on
     */
    vector<Line> l1 = getLines();
    vector<Line> l2 = shape.getLines();

    //cout << l1.size() << " " << l2.size() << endl;

    FloatRect r = getGlobalBounds();
    for (int i = 0; i < l1.size(); i++) {
        //cout << l1[i].getStart().x << " " << l1[i].getStart().y << endl;
        l1[i].offset(Vector2f(r.left, r.top));
        //cout << l1[i].getStart().x << " " << l1[i].getStart().y << endl;
    }

    r = shape.getGlobalBounds();
    //cout << r.left << " " << r.top << endl;
    for (int i = 0; i < l2.size(); i++) {
        l2[i].offset(Vector2f(r.left, r.top));
    }
    
    /*
    And now we actually check the intersection between our lines
    */

    for (int i = 0; i < l1.size(); i++) {
        for (int j = 0; j < l2.size(); j++) {
            if (l1[i].intersects(l2[j])) {
                cout << i << " " << j << endl;
                return true;
            }
        }
    }
    
    //cout << "No collision" << endl;
    return false;
}
