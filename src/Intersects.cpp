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
    
    //We first check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    FloatRect overlap;
    if (!getGlobalBounds().intersects(shape.getGlobalBounds(), overlap)) {
        //cout << "Rect bounds" << endl;
        return false;
    }
    
    //The next order of business here is that we need to grab the lines of each shape 
    vector<Line> l1 = getLines();
    vector<Line> l2 = shape.getLines();

    //And now we actually check the intersection between our lines
    for (int i = 0; i < l1.size(); i++) {
        for (int j = 0; j < l2.size(); j++) {
            if (l1[i].intersects(l2[j])) {
                //cout << i << " " << j << endl;
                // We only care about the fact that something intersects here, so we end right after
                return true;
            }
        }
    }
    
    return false;
}

bool Polygon::intersects(Polygon shape, Vector2f& resultant) {
    //We first check to make sure the two polygons are actually capable of intersecting by checking their circular boundary
    // This uses the farthest distance of each shape as the radius of a circle,
    
    float centroidDistance = sqrt(pow(getCentroid().x - shape.getCentroid().x, 2)\
     + pow(getCentroid().y - shape.getCentroid().y, 2));

    if (centroidDistance > ) {
        //cout << "Rect bounds" << endl;
        return false;
    }
    
    // Now we grab our lines
    vector<Line> l1 = getLines();
    vector<Line> l2 = shape.getLines();

    /*
    And now we actually check the intersection between our lines
    Since we want to be able to respond to a collision properly (in what direction)
    should we move the objects), we want to know which lines were actually intersecting

    These are stored in a vector of line pairs such that we can keep track of which lines
    are intersecting which ones
    We also record the points at which each pair of lines intersect
    */
    vector<Line> intersectingLines;

    for (int i = 0; i < l1.size(); i++) {
        for (int j = 0; j < l2.size(); j++) {
            if (l1[i].intersects(l2[j])) {
                //cout << i << " " << j << endl;
                intersectingLines.push_back(l2[j]);
            }
        }
    }
    
    if (intersectingLines.size() == 0)
        return false;

    // We now take the "average of all of our lines"
    // This actually means we just take the average of their slopes
    float averageSlope = 0;
    for (Line l: intersectingLines) {
        averageSlope += l.getSlope();
    }
    averageSlope /= intersectingLines.size();

    // Take the negative reciprical of the slope
    float pSlope = -1 / averageSlope;

    // Now our slope is y/x, so our vector is (1, slope)
    Vector2f perpendicular(1, pSlope);
    
    // And normalize it
    VectorMath::normalize(perpendicular);

    adjustVelocityFromCollision(perpendicular, shape);

    return true;
}

/*
We use conservation of momentum and conservation of energy (sorta) to figure out how things move
after they collide
*/
void Polygon::adjustVelocityFromCollision(Vector2f resultant, Polygon shape) {
    // Whether our collision is elastic or not
    float energyConserved = getRigidity() * shape.getRigidity();

    Vector2f initialLinearMomentum = getVelocity() * getMass() + shape.getVelocity() * shape.getMass();

    float poly1TransKE0 = (1/2) * getMass() * pow(VectorMath::mag(getVelocity()), 2);
    float poly2TransKE0 = (1/2) * shape.getMass() * pow(VectorMath::mag(shape.getVelocity()), 2);

    float poly1RotKE0 = (1/2) * getMomentOfInertia() * pow(getAngularVelocity(), 2);
    float poly2RotKE0 = (1/2) * shape.getMomentOfInertia() * pow(shape.getAngularVelocity(), 2);

    float initialKineticEnergy = poly1TransKE0 + poly1RotKE0 + poly2TransKE0 + poly2RotKE0;
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