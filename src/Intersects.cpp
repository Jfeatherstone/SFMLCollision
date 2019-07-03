#include "Polygon.hpp"

/*
We have constructors to convert other SFML shapes into polygons, so these wrappers are all going
to be exactly the same. The constructors and defined in Polygon.cpp, but essentially just copy
over the points and update a few values.
*/


/**
 * @brief A wrapper method to check the intersection between a Polygon shape and a RectangleShape
 * by converting it to a polygon and then using our full intersection method
 * See intersects(Polygon shape) for the full intersection method
 * 
 * @param shape The shape we are checking to be colliding with the current one
 * @return true The two shapes are colliding
 * @return false The two shapes aren't colliding
 */
bool Polygon::intersects(sf::RectangleShape shape) {
    Polygon poly(shape);
    return intersects(poly);
}

/**
 * @brief A wrapper method to check the intersection between a Polygon shape and a CircleShape
 * by converting it to a polygon and then using our full intersection method
 * See intersects(Polygon shape) for the full intersection method

 * @param shape The shape we are checking to be colliding with the current one
 * @return true The two shapes are colliding
 * @return false The two shapes aren't colliding
 */
bool Polygon::intersects(sf::CircleShape shape) {
    Polygon poly(shape);
    return intersects(poly);
}

/**
 * @brief A wrapper method to check the intersection between a Polygon shape and a ConvexShape
 * by converting it to a polygon and then using our full intersection method
 * See intersects(Polygon shape) for the full intersection method
 * 
 * @param shape The shape we are checking to be colliding with the current one
 * @return true The two shapes are colliding
 * @return false The two shapes aren't colliding
 */
bool Polygon::intersects(sf::ConvexShape shape) {
    Polygon poly(shape);
    return intersects(poly);
}

/*
The big one

This is the actual intersection method, that is called in all of the above "wrappers" for it
*/

/**
 * @brief Check the intersection between two polygon shapes. Has three levels of detection, to reduce
 * unecessary calculations and resource usage. Returns no information about collision after-effects
 * or resultants. For that, see intersects(Polygon shape, sf::Vector2f& resultant).
 * 
 * @param shape The shape we are checking to be colliding with the current one
 * @return true The two shapes are colliding
 * @return false The two shapes aren't colliding
 */
bool Polygon::intersects(Polygon shape) {
    
    // We first creating a circle around our objects with the radius equal to the farthest vertex distance and see if they intersect
    float centroidDistance = sqrt(pow((getPosition().x + getCentroid().x - getOrigin().x) - (shape.getPosition().x + shape.getCentroid().x - shape.getOrigin().x), 2)\
     + pow((getPosition().y - getOrigin().y + getCentroid().y) - (shape.getPosition().y - shape.getOrigin().y) + shape.getCentroid().y, 2));

    if (centroidDistance > getFarthestVertex() + shape.getFarthestVertex()) {
        //cout << "Rect bounds" << endl;
        //cout << "Vertex elim" << endl;
        return false;
    }
    
    // Next, we check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    sf::FloatRect overlap;
    if (!getGlobalBounds().intersects(shape.getGlobalBounds(), overlap)) {
        //cout << "Rect bounds" << endl;
        return false;
    }

    //The next order of business here is that we need to grab the lines of each shape 
    std::vector<Line> l1 = getLines();
    std::vector<Line> l2 = shape.getLines();

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

/**
 * @brief Currently WIP!!
 * 
 * @param shape The shape we are checking to be colliding with the current one
 * @param resultant The resultant of the collision (i.e. how the object moves afterwards)
 * @return true The two shapes are colliding
 * @return false The two shapes aren't colliding
 */
bool Polygon::intersects(Polygon& shape, sf::Vector2f& resultant) {
    //We first check to make sure the two polygons are actually capable of intersecting by checking their circular boundary
    // This uses the farthest distance of each shape as the radius of a circle,
    
    float centroidDistance = sqrt(pow((getPosition().x + getCentroid().x - getOrigin().x) - (shape.getPosition().x + shape.getCentroid().x - shape.getOrigin().x), 2)\
     + pow((getPosition().y - getOrigin().y + getCentroid().y) - (shape.getPosition().y - shape.getOrigin().y) + shape.getCentroid().y, 2));

    if (centroidDistance > getFarthestVertex() + shape.getFarthestVertex()) {
        //cout << "Rect bounds" << endl;
        //cout << "Vertex elim" << endl;
        return false;
    }
    
    // Next, we check to make sure the two polygons are actually capable of intersecting by checking their rectangular boundary
    sf::FloatRect overlap;
    if (!getGlobalBounds().intersects(shape.getGlobalBounds(), overlap)) {
        //cout << "Rect bounds" << endl;
        return false;
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
        return false;

    // We now take the "average of all of our lines"
    // This actually means we just take the average of their slopes
    float averageSlope = 0;
    for (Line l: intersectingLines) {
        //cout << l.getSlope() << endl;
        averageSlope += l.getSlope();
    }
    averageSlope /= intersectingLines.size();

    // Take the negative reciprical of the slope
    float pSlope = -1.0f / averageSlope;

    //cout << pSlope << endl;

    // Now our slope is y/x, so our vector is (1, slope)
    sf::Vector2f perpendicular(pSlope, 1);
    
    // And normalize it
    VectorMath::normalize(perpendicular);

    //cout << intersectingLines.size() << endl;

    // Since there could be more than one collision point, we want to take the average
    sf::Vector2f averageCollision(0, 0);
    for (sf::Vector2f p: intersectingPoints) {
        averageCollision += p;
    }
    averageCollision.x /= intersectingPoints.size();
    averageCollision.y /= intersectingPoints.size();

    adjustFromForce(perpendicular, averageCollision, shape);
    
    return true;
}

/*
We use conservation of momentum and conservation of energy (sorta) to figure out how things move
after they collide
*/

/**
 * @brief Currently WIP!!
 * 
 * @param resultant The unit vector in the direction of the new motion
 * @param shape The other colliding shape
 */
void Polygon::adjustFromForce(sf::Vector2f resultant, sf::Vector2f collisionPoint, Polygon& shape) {

    sf::Vector2f v1f;
    sf::Vector2f v2f;
    float w1f;
    float w2f;
    float loss = getRigidity() * shape.getRigidity();

    ///*

    // This one's really simple
    if (!isMovableByCollision() && shape.isMovableByCollision()) {
        //cout << "This immovable" << endl;
        v1f = getVelocity(); // Doesn't change
        v1f = -1.0f * VectorMath::mag(shape.getVelocity() + getVelocity()) * resultant - getVelocity();
        shape.setVelocity(loss * v2f);
        return;
    }

    if (!shape.isMovableByCollision() && isMovableByCollision()) {
        //cout << "Shape immovable" << endl;        
        v2f = shape.getVelocity(); // Doesn't change
        v1f = -1.0f * VectorMath::mag(shape.getVelocity() + getVelocity()) * resultant - shape.getVelocity();
        setVelocity(loss * v1f);
        return;
    }
    
    //*/

    // We are going to rename some variables so we can simplify the montrous equations below
    float ma = getMass();
    float mb = shape.getMass();
    float e = getRigidity() * shape.getRigidity();
    float Ia = getMomentOfInertia();
    float Ib = getMomentOfInertia();
    sf::Vector2f ra = (getPosition() - getOrigin() + getCenterOfMass()) - collisionPoint;
    sf::Vector2f rb = (shape.getPosition() - shape.getOrigin() + shape.getCenterOfMass()) - collisionPoint;
    sf::Vector2f v1i = getVelocity();
    sf::Vector2f v2i = shape.getVelocity();
    float w1i = getAngularVelocity();
    float w2i = shape.getAngularVelocity();

    std::cout << rb.x << " " << rb.y << std::endl;

    sf::Vector2f impulse = 2.0f * (ma*mb)/(ma+mb)*VectorMath::dot((v1i) - (v2i), resultant) * resultant;

    v1f = v1i - impulse / ma;
    v2f = v2i + impulse / mb;

    w1f = w1i - VectorMath::cross(impulse, ra) / ma;
    w2f = w2i + VectorMath::cross(impulse, ra) / mb;

    //cout << v2f.x << " " << v2f.y << endl;

    // Make sure we don't get nan
    if ((v1f.x >= 0 || v1f.x <= 0) && (v1f.y >= 0 || v1f.y <= 0) && (v2f.x >= 0 || v2f.x <= 0) && (v2f.y >= 0 || v2f.y <= 0)) {
        if (isMovableByCollision())
            setVelocity(loss*v1f);
        if (shape.isMovableByCollision())
            shape.setVelocity(loss*v2f);
    }

    if ((w1f >= 0 || w1f <= 0) && (w2f >= 0 || w2f <= 0)) {
        if (isMovableByCollision())
            setAngularVelocity(loss*w1f);
        if (shape.isMovableByCollision())
            shape.setAngularVelocity(loss*w2f);
    }

    /*
    const float MIN_DISPLACEMENT = .01f;

    float poly1TransKE0 = (.5f) * getMass() * pow(VectorMath::mag(getVelocity()), 2);
    float poly2TransKE0 = (.5f) * shape.getMass() * pow(VectorMath::mag(shape.getVelocity()), 2);

    float initialKineticEnergy = poly1TransKE0 + poly2TransKE0;

    // We want to model things as springs, with a displacement based on the rigidity of both shapes
    float displacement = MIN_DISPLACEMENT / (pow(getRigidity() * shape.getRigidity(), 2));

    VectorMath::normalize(resultant, displacement);

    float k = 2.0f * initialKineticEnergy / (displacement*displacement);

    //cout << k << endl;

    sf::Vector2f averageForce = -.5f * k * resultant;
    
    float w = sqrt(k/getMass());

    sf::Vector2f impulse = averageForce * (3.14f)/w;

    //cout << impulse.x << " " << impulse.y << endl;

    v1f.x = (getMass() * getVelocity().x + impulse.x) / getMass();
    v1f.y = (getMass() * getVelocity().y + impulse.y) / getMass();
    
    v2f.x = (shape.getMass() * shape.getVelocity().x - impulse.x) / shape.getMass();
    v2f.y = (shape.getMass() * shape.getVelocity().y - impulse.y) / shape.getMass();


    float loss = getRigidity() * shape.getRigidity();

    //VectorMath::normalize(v1f, loss * VectorMath::mag((shape.getMass() / (shape.getMass() + getMass())) * (shape.getVelocity() + getVelocity())));
    //VectorMath::normalize(v2f, loss * VectorMath::mag((getMass() / (shape.getMass() + getMass())) * (getVelocity() + shape.getVelocity())));
    VectorMath::normalize(v1f, loss * VectorMath::mag(getVelocity()));
    VectorMath::normalize(v2f, loss * VectorMath::mag(shape.getVelocity()));

    

    // Make sure we don't get nan
    if ((v1f.x >= 0 || v1f.x <= 0) && (v1f.y >= 0 || v1f.y <= 0)) {
        setVelocity(v1f);
        shape.setVelocity(v2f);
    }
    */

    /*
    // Whether our collision is elastic or not
    float energyConserved = getRigidity() * shape.getRigidity();

    // The values we are solving for
    float v1f = 0;
    float v2f = 0;
    float w1f = 0;
    float w2f = 0;

    sf::Vector2f initialLinearMomentum = getVelocity() * getMass() + shape.getVelocity() * shape.getMass();

    float initialAngularMomentum = getAngularVelocity()*getMomentOfInertia() + shape.getAngularVelocity()*shape.getMomentOfInertia();

    float poly1TransKE0 = (1/2) * getMass() * pow(VectorMath::mag(getVelocity()), 2);
    float poly2TransKE0 = (1/2) * shape.getMass() * pow(VectorMath::mag(shape.getVelocity()), 2);

    float poly1RotKE0 = (1/2) * getMomentOfInertia() * pow(getAngularVelocity(), 2);
    float poly2RotKE0 = (1/2) * shape.getMomentOfInertia() * pow(shape.getAngularVelocity(), 2);

    float initialKineticEnergy = poly1TransKE0 + poly1RotKE0 + poly2TransKE0 + poly2RotKE0;

    float forceShape1On2 = 1;
    float forceShape2On1 = -forceShape1On2;

    // Since we need to solve for 4 variables (v1f, v2f, w1f, w2f) but only have 3 equations,
    // we can approximate that after a collision, on one object will be rotating
    // We decide which object rotates by which is hit further from their centroid (more torque)

    float averageDistance1 = 0;
    float averageDistance2 = 0;

    for (sf::Vector2f v: collisionPoints) {

        averageDistance1 += VectorMath::mag(getCentroid() + (getPosition() - getOrigin()) - v);
        averageDistance2 += VectorMath::mag(shape.getCentroid() + (shape.getPosition() - shape.getOrigin()) - v);
    }

    averageDistance1 /= collisionPoints.size();
    averageDistance2 /= collisionPoints.size();

    bool thisRotates = averageDistance1 > averageDistance2;

    // We can then easily calculate both angular velocities
    if (thisRotates) {
        w2f = 0;
        w1f = initialAngularMomentum / getMomentOfInertia();
    } else {
        w1f = 0;
        w2f = initialAngularMomentum / shape.getMomentOfInertia();
    }

    float finalAngularKineticEnergy = (1/2) * shape.getMomentOfInertia() * pow(w2f, 2) + (1/2) * getMomentOfInertia() * pow(w1f, 2);

    // Now, by solving our systems of equations, we get the following quadratic
    float c = (1/2) * pow(VectorMath::mag(initialLinearMomentum), 2) / getMass() + finalAngularKineticEnergy - initialKineticEnergy;
    float b = - VectorMath::mag(initialLinearMomentum) * shape.getMass() / getMass();
    float a = (1/2) * (pow(shape.getMass(), 2)/getMass() + shape.getMass());

    float solutions[2] = {(-b + sqrt(pow(b, 2) - 4*a*c))/(2*a), (-b - sqrt(pow(b, 2) - 4*a*c))/(2*a)};
    cout << solutions[0] << " " << solutions[1] << endl;
    //*/

}

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

}