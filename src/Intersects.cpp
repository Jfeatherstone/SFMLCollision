#include "Polygon.hpp"

///////////////////////////////////////
//          INTERSECTION
///////////////////////////////////////

/*
The big one

This is the actual intersection method, that is called in all of the above "wrappers" for it
*/
std::vector<sf::Vector2u> Polygon::intersects(Polygon shape) {
    
    std::vector<sf::Vector2u> intersectingLines;

    // We first creating a circle around our objects with the radius equal to the farthest vertex distance and see if they intersect
    /*
    sf::Vector2f poly1CentroidPosition(getPosition().x + (getCentroid().x - getOrigin().x) * getScale().x,
                                        getPosition().y + (getCentroid().y - getOrigin().y) * getScale().y);

    sf::Vector2f poly2CentroidPosition(shape.getPosition().x + (shape.getCentroid().x - shape.getOrigin().x) * shape.getScale().x,
                                        shape.getPosition().y + (shape.getCentroid().y - shape.getOrigin().y) * shape.getScale().y);

    float centroidDistance = sqrt(pow(poly2CentroidPosition.x - poly1CentroidPosition.x, 2) + pow(poly2CentroidPosition.y - poly1CentroidPosition.y, 2));

    float poly1Diagonal = sqrt(getLocalBounds().width * getLocalBounds().width / 4 + 
                                getLocalBounds().height * getLocalBounds().height / 4);
    float poly2Diagonal = sqrt(shape.getLocalBounds().width * shape.getLocalBounds().width / 4 + 
                                shape.getLocalBounds().height * shape.getLocalBounds().height / 4);
    
    if (centroidDistance > poly1Diagonal + poly2Diagonal) {
        //cout << "Rect bounds" << endl;
        //cout << "Vertex elim" << endl;
        return intersectingLines;
    }
    */

    // Next, we check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    if (!getGlobalBounds().intersects(shape.getGlobalBounds())) {
        //cout << "Rect bounds" << endl;
        return intersectingLines;
    }

    //The next order of business here is that we need to grab the lines of each shape 
    std::vector<Line> l1 = getLines();
    std::vector<Line> l2 = shape.getLines();

    //And now we actually check the intersection between our lines
    for (int i = 0; i < l1.size(); i++) {
        for (int j = 0; j < l2.size(); j++) {
            if (l1[i].intersects(l2[j])) {
                //std::cout << i << " " << j << std::endl;
                intersectingLines.push_back(sf::Vector2u(i, j));
            }
        }
    }
    
    return intersectingLines;
}

/*
We have constructors to convert other SFML shapes into polygons, so these wrappers are all going
to be exactly the same. The constructors and defined in Polygon.cpp, but essentially just copy
over the points and update a few values.
*/

bool Polygon::intersects(sf::RectangleShape shape) {
    Polygon poly(shape);
    //return intersects(poly);
    return false;
}

bool Polygon::intersects(sf::CircleShape shape) {
    Polygon poly(shape);
    //return intersects(poly);
    return false;
}

bool Polygon::intersects(sf::ConvexShape shape) {
    Polygon poly(shape);
    //return intersects(poly);
    return false;
}

/**
 * @brief Currently WIP!!
 * 
 * @param shape The shape we are checking to be colliding with the current one
 * @return true The two shapes are colliding
 * @return false The two shapes aren't colliding
 */
std::vector<sf::Shape*> Polygon::intersectAndResolve(Polygon& shape) {
    
    std::vector<sf::Shape*> vec;

    // Next, we check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    sf::FloatRect overlap;
    if (!getGlobalBounds().intersects(shape.getGlobalBounds(), overlap)) {
        //cout << "Rect bounds" << endl;
        //return false;
        return vec;
    }
    
    // Now we grab our lines
    std::vector<Line> l1 = getLines();
    std::vector<Line> l2 = shape.getLines();

    /*
    And now we actually check the intersection between our lines
    Since we want to be able to respond to a collision properly (in what direction)
    should we move the objects), we want to know which lines were actually intersecting

    These are stored in a vector of line pairs such that we can keep track of which lines
    are intersecting which ones
    We also record the points at which each pair of lines intersect
    */
    std::vector<Line> intersectingLines;
    std::vector<sf::Vector2f> intersectingPoints;

    for (int i = 0; i < l1.size(); i++) {
        for (int j = 0; j < l2.size(); j++) {
            sf::Vector2f p;
            if (l1[i].intersects(l2[j], p)) {
                //cout << i << " " << j << endl;
                intersectingLines.push_back(l2[j]);
                intersectingPoints.push_back(p);
            }
        }
    }

    if (intersectingLines.size() == 0)
        //return false;
        return vec;

    // Since there could be more than one collision point, we want to take the average
    sf::Vector2f averageCollision(0, 0);
    for (sf::Vector2f p: intersectingPoints) {
        sf::CircleShape* c = new sf::CircleShape();
        c->setRadius(5);
        c->setPosition(p);
        c->setFillColor(sf::Color::Red);
        c->setOrigin(Polygon(*c).getCentroid());
        vec.push_back(c);

        averageCollision += p;
    }

    averageCollision.x /= intersectingPoints.size();
    averageCollision.y /= intersectingPoints.size();

    // We now take the "average of all of our lines"
    // This actually means we just take the average of their slopes
    sf::Vector2f averageSlope;

    int i = 0;

    for (Line l: intersectingLines) {
        //cout << l.getSlope() << endl;
        averageSlope += l.getPerpendicular();
        std::cout << l.getPerpendicular().x << " " << l.getPerpendicular().y << std::endl;
        vec.push_back(Line(intersectingPoints[i], intersectingPoints[i] + VectorMath::normalize(l.getPerpendicular(), 50)).getDrawable(sf::Color::Green));
        i++;
    }

    averageSlope.x /= intersectingLines.size();
    averageSlope.y /= intersectingLines.size();

    // Take the negative reciprical of the slope
    float pSlope = averageSlope.y / averageSlope.x;

    //cout << pSlope << endl;

    // Now our slope is y/x, so our vector is (1, slope)
    sf::Vector2f normal(pSlope, (pSlope == 0) ? 1.0f : abs(pSlope) / pSlope);
    
    // And normalize it
    VectorMath::normalize(normal);

    //std::cout << normal.x << " " << normal.y << std::endl;

    Line l(averageCollision, averageCollision + normal * 100.0f);

    sf::CircleShape* c = new sf::CircleShape();
    c->setRadius(5);
    c->setPosition(averageCollision);
    c->setOrigin(Polygon(*c).getCentroid());
    c->setFillColor(sf::Color::Blue);
    
    sf::Vector2f poly1CentroidPosition(getPosition().x + (getCentroid().x - getOrigin().x) * getScale().x,
                                        getPosition().y + (getCentroid().y - getOrigin().y) * getScale().y);

    sf::Vector2f poly2CentroidPosition(shape.getPosition().x + (shape.getCentroid().x - shape.getOrigin().x) * shape.getScale().x,
                                        shape.getPosition().y + (shape.getCentroid().y - shape.getOrigin().y) * shape.getScale().y);

    sf::Vector2f centroidDistance = poly2CentroidPosition - poly1CentroidPosition;

    sf::Vector2f poly1CentroidToCollision = (getPosition() - sf::Vector2f(getOrigin().x * getScale().x, getOrigin().y * getScale().y) 
            + sf::Vector2f(getCenterOfMass().x * getScale().x, getCenterOfMass().y * getScale().y)) - averageCollision;

    sf::Vector2f poly2CentroidToCollision = (shape.getPosition() - sf::Vector2f(shape.getOrigin().x * shape.getScale().x, shape.getOrigin().y * shape.getScale().y) 
            + sf::Vector2f(shape.getCenterOfMass().x * shape.getScale().x, shape.getCenterOfMass().y * shape.getScale().y)) - averageCollision;

    sf::Vector2f penetration = centroidDistance - poly1CentroidToCollision - poly2CentroidToCollision;

    //vec.push_back(Line((getPosition() - sf::Vector2f(getOrigin().x * getScale().x, getOrigin().y * getScale().y) 
    //        + sf::Vector2f(getCenterOfMass().x * getScale().x, getCenterOfMass().y * getScale().y)), averageCollision).getDrawable(sf::Color::White));

    //vec.push_back(Line((shape.getPosition() - sf::Vector2f(shape.getOrigin().x * shape.getScale().x, shape.getOrigin().y * shape.getScale().y) 
    //        + sf::Vector2f(shape.getCenterOfMass().x * shape.getScale().x, shape.getCenterOfMass().y * shape.getScale().y)), averageCollision).getDrawable(sf::Color::White));

    vec.push_back(c);
    vec.push_back(l.getDrawable(sf::Color::Red));

    return vec;

    /*
    sf::Vector2f poly1MomentVector = 
        getMomentOfInertia() * ((getPosition() - sf::Vector2f(getOrigin().x * getScale().x, getOrigin().y * getScale().y) 
            + sf::Vector2f(getCenterOfMass().x * getScale().x, getCenterOfMass().y * getScale().y))
            - averageCollision);
    
    //std::cout << poly1MomentVector.x << " " << poly1MomentVector.y << std::endl;

    sf::Vector2f poly2MomentVector = 
        shape.getMomentOfInertia() * ((shape.getPosition() - sf::Vector2f(shape.getOrigin().x * shape.getScale().x, shape.getOrigin().y * shape.getScale().y) 
            + sf::Vector2f(shape.getCenterOfMass().x * shape.getScale().x, shape.getCenterOfMass().y * shape.getScale().y))
            - averageCollision);

    //std::cout << poly2MomentVector.x << " " << poly2MomentVector.y << std::endl;

    float coeffOfRestitution = (getYoungsModulus() + shape.getYoungsModulus());

    float numerator = coeffOfRestitution * 
        VectorMath::dot(shape.getVelocity() - getVelocity() + shape.getAngularVelocity() * poly2MomentVector - getAngularVelocity() * poly1MomentVector, normal);

    float denominator = (1 / getMass() + 1 / shape.getMass())
         + (1 / getMomentOfInertia() * pow(VectorMath::dot(poly1MomentVector, normal), 2)) + (1 / shape.getMomentOfInertia() * pow(VectorMath::dot(poly2MomentVector, normal), 2));

    float impulse = numerator / denominator;

    //std::cout << impulse << std::endl;

    sf::Vector2f poly1Vf = getVelocity() + impulse * normal / getMass();
    sf::Vector2f poly2Vf = shape.getVelocity() - impulse * normal / shape.getMass();

    vec.push_back(Line(averageCollision, averageCollision + impulse * normal / getMass()).getDrawable(sf::Color::Blue));

    //std::cout << poly1Vf.x << " " << poly1Vf.y << std::endl;
    //std::cout << poly2Vf.x << " " << poly2Vf.y << std::endl;

    float poly1Wf = getAngularVelocity() - impulse * VectorMath::dot(poly1MomentVector, normal) / getMomentOfInertia();
    float poly2Wf = shape.getAngularVelocity() - impulse * VectorMath::dot(poly2MomentVector, normal) / shape.getMomentOfInertia();

    setVelocity(poly1Vf);
    shape.setVelocity(poly2Vf);
    setAngularVelocity(poly1Wf);
    shape.setAngularVelocity(poly2Wf);
    */

    ///*
    ///////////////////////////////////////////////////////
    //       CIRCLES
    // The following code works for resolving collsiions
    // between circles (but only circles and even then it has issues)
    ///////////////////////////////////////////////////////

    float coeffOfRestitution = (getYoungsModulus() + shape.getYoungsModulus());

    float totalVel = (getLinearFreedom() ? VectorMath::mag(getVelocity()) : 0.f) + (shape.getLinearFreedom() ? VectorMath::mag(shape.getVelocity()) : 0.f) + VELOCITY_THRESHOLD;

    VectorMath::normalize(poly1CentroidToCollision);
    VectorMath::normalize(poly2CentroidToCollision);

    float totalMass = (getLinearFreedom() ? getMass() : 0.f) + (shape.getLinearFreedom() ? shape.getMass() : 0.f);

    sf::Vector2f poly1Vf = getMass() / totalMass * totalVel * poly1CentroidToCollision * coeffOfRestitution / 2.0f;
    sf::Vector2f poly2Vf = shape.getMass() / totalMass * totalVel * poly2CentroidToCollision * coeffOfRestitution / 2.0f;

    if (getLinearFreedom())
        setVelocity(poly1Vf);
    if (shape.getLinearFreedom())
        shape.setVelocity(poly2Vf);

    return vec;
    //*/

    return vec;
    //return true;
}

/*
We use conservation of momentum and conservation of energy (sorta) to figure out how things move
after they collide
*/

//////////////////////////////////////////
// CONTAINS
//////////////////////////////////////////

// These next few methods are our wrappers and are more or less copied from our wrappers
// for intersection

/**
 * @brief Not implemented!
 * 
 * @param shape 
 * @return true 
 * @return false 
 */
bool Polygon::contains(sf::RectangleShape shape) {
    Polygon poly(shape);
    return contains(poly);
}

/**
 * @brief Not implemented!
 * 
 * @param shape 
 * @return true 
 * @return false 
 */
bool Polygon::contains(sf::CircleShape shape) {
    Polygon poly(shape);
    return contains(poly);
}

/**
 * @brief Not implemented!
 * 
 * @param shape 
 * @return true 
 * @return false 
 */
bool Polygon::contains(sf::ConvexShape shape) {
    Polygon poly(shape);
    return contains(poly);
}

/*
The actual method that will be called by the above wrappers

First, we check the width and height of the parameter shape against this one. If the other has a
larger height or width, it cannot be inside this, and we return false

Now we actually do the real calculation:
If we take any given line of the object we suspect to be insde of the other, we can extend that segment to be a full
line and find how many times it intersects with the outer shape. If this amount is an integer multiple of 2, and at
least one intersection point is on either side of the shape, the shape must be inside of the other.

*/

/**
 * @brief Not implemented!
 * 
 * @param shape 
 * @return true 
 * @return false 
 */
bool Polygon::contains(Polygon shape) {
    return false;
}