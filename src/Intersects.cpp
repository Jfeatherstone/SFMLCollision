#include "Polygon.hpp"

/*
We have constructors to convert other SFML shapes into polygons, so these wrappers are all going
to be exactly the same. The constructors and defined in Polygon.cpp, but essentially just copy
over the points and update a few values.
*/
bool Polygon::intersects(RectangleShape shape) {
    Polygon poly(shape);
    return intersects(poly);
}

bool Polygon::intersects(CircleShape shape) {
    Polygon poly(shape);
    return intersects(poly);
}

bool Polygon::intersects(ConvexShape shape) {
    Polygon poly(shape);
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
    
    FloatRect overlap;
    if (!getGlobalBounds().intersects(shape.getGlobalBounds(), overlap)) {
        cout << "Rect bounds" << endl;
        return false;
    }

    /*
    The next order of business here is that we need to grab the lines of each shape
     */
    vector<Line> l1 = getLines();
    vector<Line> l2 = shape.getLines();

    
    /*
    And now we actually check the intersection between our lines
    */
    for (int i = 0; i < l1.size(); i++) {
        for (int j = 0; j < l2.size(); j++) {
            if (l1[i].intersects(l2[j])) {
                //cout << i << " " << j << endl;
                // We only care about the fact that something intersects here, so we end right after
                return true;
            }
        }
    }
    
    //cout << "No collision" << endl;
    return false;
}

bool Polygon::intersects(Polygon shape, Vector2f& resultant) {
    //We first check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    FloatRect overlap;
    if (!getGlobalBounds().intersects(shape.getGlobalBounds(), overlap)) {
        //cout << "Rect bounds" << endl;
        return false;
    }
    
    // Now we grab our lines
    vector<Line> l1 = getLines();
    vector<Line> l2 = shape.getLines();

    //cout << l1.size() << " " << l2.size() << endl;    
    /*
    And now we actually check the intersection between our lines
    Since we want to be able to respond to a collision properly (in what direction)
    should we move the objects), we want to know which lines were actually intersecting

    These are stored in a two dimensional vector such that we can keep track of which lines
    are intersecting which ones
    */
    vector<vector<Line>> intersectingLines;

    for (int i = 0; i < l1.size(); i++) {
        for (int j = 0; j < l2.size(); j++) {
            if (l1[i].intersects(l2[j])) {
                //cout << i << " " << j << endl;
                return true;
            }
        }
    }
    
    //cout << "No collision" << endl;
    return false;
}

//////////////////////////////////////////
// CONTAINS
//////////////////////////////////////////

// These next few methods are our wrappers and are more or less copied from our wrappers
// for intersection
bool Polygon::contains(RectangleShape shape) {
    Polygon poly(shape);
    return contains(poly);
}

bool Polygon::contains(CircleShape shape) {
    Polygon poly(shape);
    return contains(poly);
}

bool Polygon::contains(ConvexShape shape) {
    Polygon poly(shape);
    return contains(poly);
}

/*
The actual method that will be called by the above wrappers

First, e check the width and height of the parameter shape against this one. If the other has a
larger height or width, it cannot be inside this, and we return false

Now we actually do the real calculation:
If we take any given line of the object we suspect to be insde of the other, we can extend that segment to be a full
line and find how many times it intersects with the outer shape. If this amount is an integer multiple of 2, and at
least one intersection point is on either side of the shape, the shape must be inside of the other.

*/
bool Polygon::contains(Polygon shape) {

}