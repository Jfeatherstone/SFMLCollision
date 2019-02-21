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
                cout << i << " " << j << endl;
                // We only care about the fact that something intersects here, so we end right after
                return true;
            }
        }
    }
    
    //cout << "No collision" << endl;

    /*
    Since we know that our shapes have likely some overlap, we should check to see if one shape is inside of the other

    If we take any given line of the object we suspect to be insde of the other, we can extend that segment to be a full
    line and find how many times it intersects with the outer shape. If this amount is an integer multiple of 4N + 2, then
    the shape must be inside of the other. We also have to make sure that at least one intersection point is on either side
    of the shape.
    */

    /*
    To decide which shape could be inside the other (as only one could be) we check the width and height of each shape against
    each other. If one has a larger height and the other a larger width, one cannot be inside the other, and so both conditions
    must be true for either shape
    */ 

    //cout << getGlobalBounds().width << " " << getGlobalBounds().height << endl;
    //cout << shape.getGlobalBounds().width << " " << shape.getGlobalBounds().height << endl;

    if (getGlobalBounds().width < shape.getGlobalBounds().width && getGlobalBounds().height < shape.getGlobalBounds().height
        && isSolid()) {
        cout << "This inside" << endl;
        // The case where this is the outer shape
        
        // We can just grab the first line as our test (l1)
        Line l = l1[0];

        // Now we get a list of intersections with the lines ffrom the other polygon (shape)
        vector<Line> intersections;
        vector<Vector2f> intersectionPoints;

        for (Line line: l2) {
            Vector2f point;
            if (l.intersects(line, point, true)) {
                intersections.push_back(line);
                intersectionPoints.push_back(point);
            }
        }

        if ((intersections.size() - 2) % 4 == 0) {
            cout << "Size OK (" << intersections.size() << ")" << endl;
            /*
            Now we want to make sure that there is at least one point on either side of the shape.
            Since we have a 2 dimensional space, this isn't as easy as just comparing x values, since "either side"
            could mean one above and one below.
            What we can say, is that any intersection have to be along our line, which means they are 180 degrees from each
            other. Because of this, we can calculate which quadrant (with respect to the shapes centroid) each point is in
            and if there are at least two in opposing quadrants (1 and 3, or 2 and 4), then it has one on "either side".
            */
            bool foundQuadrants[4];
            for (int i = 0; i < intersections.size(); i++) {
                foundQuadrants[VectorMath::quadrant(intersectionPoints[i], getCentroid()) - 1] = true;
            }
            int numQuads = 0;
            for (bool b: foundQuadrants) {
                if (b)
                    numQuads++;
            }
            cout << "Found points in " << numQuads << " quadrants [" << foundQuadrants[0] << ", " << foundQuadrants[1] << ", " 
                << foundQuadrants[2] << ", " << foundQuadrants[3] << "]" << endl;
            if (numQuads > 1) {
                cout << "This inside of shape" << endl;
                return true;
            }
        }
    } else if ((getGlobalBounds().width > shape.getGlobalBounds().width && getGlobalBounds().height > shape.getGlobalBounds().height)
       && shape.isSolid()) {
        cout << "Shape inside" << endl;
        // The case where shape is the outer shape
        
        // We can just grab the first line as our test (l1)
        Line l = l2[0];

        // Now we get a list of intersections with the lines from the other polygon (this)
        vector<Line> intersections;
        vector<Vector2f> intersectionPoints;

        for (Line line: l1) {
            Vector2f point;
            if (l.intersects(line, point)) {
                intersections.push_back(line);
                intersectionPoints.push_back(point);
            }
        }
    
        cout << intersections.size() << endl;
        /*
        Now we want to make sure that there is at least one point on either side of the shape.
        Since we have a 2 dimensional space, this isn't as easy as just comparing x values, since "either side"
        could mean one above and one below.
        What we can say, is that any intersection have to be along our line, which means they are 180 degrees from each
        other. Because of this, we can calculate which quadrant (with respect to the shapes centroid) each point is in
        and if there are at least two in opposing quadrants (1 and 3, or 2 and 4), then it has one on "either side".
        */
        bool foundQuadrants[4];
        for (int i = 0; i < intersections.size(); i++) {
            foundQuadrants[VectorMath::quadrant(intersectionPoints[i], getCentroid())];
        }

        if ((foundQuadrants[0] && foundQuadrants[2]) || (foundQuadrants[1] && foundQuadrants[3])) {
            // We now use our multiple of 4N + 2 that we showed earlier
            if (((intersections.size() - 2) / 4) % 4 == 0)
                return true;
        }

}        

    return false;
}

bool Polygon::intersects(Polygon shape, Vector2f& resultant) {
    
    /*
    We first check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    */
    /*
    FloatRect overlap;
    if (!getGlobalBounds().intersects(shape.getGlobalBounds(), overlap)) {
        //cout << "Rect bounds" << endl;
        return false;
    } else {
        /*
        If the entire overlap area is equal to one of the shapes, then one must be entirely contained within the other,
        and therefore the shape are colliding
        
        if (overlap == getGlobalBounds() || overlap == shape.getGlobalBounds())
            return false;
    }
    */
    /*
    The next order of business here is that we need to adjust our lines to be relative to the window they are
    drawn on
     */
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