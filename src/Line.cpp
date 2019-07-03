#include "Line.hpp"

/*
    This value is to denote a vertical line, which would otherwise have a slope of nan, or infinity

The only importance in the value is that no other line should every organically have this slope, and that
this is an arbitrarily high number, to simulate a vertical line without actually being one
*/
float Line::VERTICAL_SLOPE = 100000;
float Line::PARALLEL_LINE_SLOPE_TOLERANCE = .01f;
float Line::PARALLEL_LINE_DISTANCE_TOLERANCE = .01f;

/*
    CONSTRUCTORS
*/

/**
 * @brief Construct a new Line object from two endpoints. They don't necessarily have to be in order
 * of higher/lower x or y
 * 
 * @param p1 Endpoint 1
 * @param p2 Endpoint 2
 */
Line::Line(sf::Vector2f p1, sf::Vector2f p2) {
    // This automatically assigns our slope member variable so we don't need to set it here

    if (p1.x == p2.x)
        m_vertical = true;
    if (p1.y == p2.y)
        m_horizontal = true;

    calculateSlope(p1, p2);

    m_start = p1;
    m_end = p2;

    calculateQuadrant();
    calculateIntercept();
    calculateAngle();
}

/**
 * @brief Construct a new Line object with no information, using for comparing lines to see if they exist
 * 
 */
Line::Line() {

}

/*
    SLOPE
    
This is a simple y2-y1/x2-x1 calculation
*/

/**
 * @brief Simple y2-y1/x2-x1 calculation to find the slope of the line. There are special cases for
 * if the line is vertical or horizontal
 * 
 * @param p1 The first point
 * @param p2 The second point
 * @return float The value of the slope
 */
float Line::calculateSlope(sf::Vector2f p1, sf::Vector2f p2) {
    if (m_horizontal) {
        m_slope == 0;
        return m_slope;
    }
    if (m_vertical) {
        m_slope = VERTICAL_SLOPE;
        return m_slope;
    }

    m_slope = (p2.y-p1.y)/(p2.x-p1.x);
    return m_slope;
}

/*
    INTERCEPT

This will use y = mx + b to find the intercept
*/

/**
 * @brief Use y = mx + b to find the intercept of our object
 * 
 * @return float The intercept of our object
 */
float Line::calculateIntercept() {
    m_intercept = m_start.y - m_slope*m_start.x;
    return m_intercept;
}

/*
    QUADRANT
*/

/**
 * @brief Find which quadrant the line is in? I honestly don't remember coding this and I'm not
 * sure it is used or not
 * 
 * @return int A quadrant number (1-4) that means... something?
 */
int Line::calculateQuadrant() {
    sf::Vector2f v = m_end - m_start;
    if (v.x > 0 && v.y > 0)
        m_quadrant = 0;
    if (v.x < 0 && v.y > 0)
        m_quadrant = 1;
    if (v.x < 0 && v.y < 0)
        m_quadrant = 2;
    if (v.x > 0 && v.y < 0)
        m_quadrant = 3;
    
    return m_quadrant;
}
/*
    ANGLE

Depending on the value for the slope, we calculate the angle differently, based on which quadrant
the line runs through

For this calculate we assume that intercept is 0, i.e. our coordinate origin is situated on top of the
line.

It is also important to note that we convert everything into degrees, because Transofrmable's setRotation() method takes
the angle value in degrees.
*/

/**
 * @brief Find the angle of our line with respect to the horizontal
 * 
 * @return float The angle
 */
float Line::calculateAngle() {

    /*
    The first thing we want to deal with are the edge cases for lines: horizontal and vertical

    These have to be handled separately because the slope of a vertical line would be nan if allowed
    to be calculated the other way, and these calculations would make no sense. 

    One other thing to not is that even though we treat 180 and 0 as different values, they actually represent the
    same exact line/rotation. This may not be true in reality, but for our limited scope -- since a line is symmetric --
    the differentiation is for the sake of clarity only. Same deal with 270 and 90 below.
    */
    if (m_horizontal) {
        if (m_start.x > m_end.x)
            m_angle = 180;
        else
            m_angle = 0;

        return m_angle;
    }

    if (m_vertical) {
        if (m_start.y > m_end.y)
            m_angle = 90;
        else
            m_angle = 270;
        
        return m_angle;
    }
    
    /*
    This next part accounts for any angle that isn't an integer multiple of PI/2.

    Depending on what quadrant our line is in, we have to do a few adjustments to the actual angle, since we are
    setting 0 degrees to be on the x-axis in a cartesian plane.
    */

    // We first act as though the start point is our origin (0, 0), and calculate what the second points would be
    sf::Vector2f v = m_end - m_start;

    switch (m_quadrant) {
        case 0:
            m_angle = atan(abs(v.y/v.x)) * 180 /M_PI;
            break;
        case 1:
            m_angle = 180 - atan(abs(v.y/v.x)) * 180 /M_PI;
            break;
        case 2:
            m_angle = 180 + atan(abs(v.y/v.x)) * 180 /M_PI;
            break;
        case 3:
            m_angle = 360 - atan(abs(v.y/v.x)) * 180 /M_PI;
            break;
    }

    // NOTE: we longer want our answer in radians
    //m_angle *= M_PI / 180; // Convert back to radians
    
    return m_angle;
}


/*
    Y VALUE

This will give the y value at any given x on the line
*/

/**
 * @brief Find the y value at any given x on the line (doesn't deal with endpoints/domain)
 * 
 * @param x Our x value
 * @return float The y value
 */
float Line::y(float x) {
    return m_slope*x + m_intercept;
}

/*
    INTERSECTS

We use the following method for line intersection:
http://ahinson.com/algorithms_general/Sections/Geometry/ParametricLineIntersection.pdf

In summary, we calculate the percent distance from the endpoints on each line (s, t) and if
both values are between 0 and 1, the intersection must occur within both domains
*/

/**
 * @brief Check to see if two lines intersect within their domain (between the endpoints)
 * 
 * @param line The other line we are checking
 * @param intersectionPoint A reference to a point that will hold the intersection point if it exist, or (-1, -1) if it doesn't
 * @param extendLine If this is true, we extend the first line from a line segment to an infinite line and check collision with that
 * @return true The two lines are intersecting
 * @return false The two lines are not intersecting
 */
bool Line::intersects(Line line, sf::Vector2f& intersectionPoint, bool extendLine) {
    // We do this to help with naming and keeping our variables organized
    // from the reference linked above (ik, 1 indexing is gross)
    float x[5] = {0, getStart().x, getEnd().x, line.getStart().x, line.getEnd().x};
    float y[5] = {0, getStart().y, getEnd().y, line.getStart().y, line.getEnd().y};

    // We precompute the denominator to detect parallel lines and to save on resources
    float denominator = ((x[4] - x[3])*(y[2] - y[1]) - (x[2] - x[1])*(y[4] - y[3]));

    // Check for parallel lines
    if (denominator < PARALLEL_LINE_SLOPE_TOLERANCE) {
        // The only way parallel lines intersect is if they are the same line
        sf::Vector2f centerA = (getStart() + getEnd()) / 2.0f;
        sf::Vector2f centerB = (line.getStart() + line.getEnd()) / 2.0f;
        sf::Vector2f distance = centerA - centerB;

        if (abs(distance.x) < PARALLEL_LINE_DISTANCE_TOLERANCE || abs(distance.y) < PARALLEL_LINE_DISTANCE_TOLERANCE) {
            intersectionPoint = centerA;
            return true;
        } else
            return false;
    }

    float s = ((x[4]-x[3])*(y[3]-y[1]) - (x[3]-x[1])*(y[4] - y[3])) / denominator;

    float t = ((x[2]-x[1])*(y[3]-y[1]) - (x[3]-x[1])*(y[2] - y[1])) / denominator;

    // This denotes whether or not the lines intersect within their domain
    // Even if they don't, we check if we are extending the line 
    // In this case we only need to check the other line's domain
    if ((s >= 0 && s <= 1 && t >= 0 && t <= 1) || (extendLine && t >= 0 && t <= 1)) {
        // We now calculate the intersection point
        intersectionPoint.x = x[1] + (x[2] - x[1])*s;
        intersectionPoint.y = y[1] + (y[2] - y[1])*s;

        return true;
    }
    
    // Otherwise, they aren't intersecting
    return false;
}

/**
 * @brief Wrapper for our more involved intersection method that doesn't deal with any other parameters but the line
 * 
 * @param line The other line
 * @return true The two lines are intersecting
 * @return false The two lines are not intersecting
 */
bool Line::intersects(Line line) {
    sf::Vector2f v;
    return intersects(line, v);
}

/*
    OFFSET

Since we will likely need to offset our lines because our lines will not always be at (0, 0), we have a method to 
properly store and change the offset.

We store this value as a member variable, so that if we need to offset again, we don't have to account for the previous offset
For example:

someLine.offset(sf::Vector2f(10, 10));
// The origin of our coordinate system will be set at (10, 10)

someLine.offset(sf::Vector2f(20, 20));
// The origin of our coordinate system will be set at (20, 20), NOT (30, 30)
*/

/**
 * @brief Offset the endpoints of our line (and recalculate the intercept) in a way that does NOT stack with previous offsets
 * (See source for more info)
 * 
 * @param offset The amount we want to offset x and y by
 */
void Line::offset(sf::Vector2f offset) {
    // First we remove the previous offset
    m_start -= m_offset;
    m_end -= m_offset;

    // Now we store the new offset and add it to the points
    m_offset = offset;

    m_start += m_offset;
    m_end += m_offset;

    // And recalculate the intercept
    calculateIntercept();
}

/*
    GETTERS
*/

/**
 * @brief Return the angle the line makes with the horizontal x axis
 * 
 * @return float The line's angle
 */
float Line::getAngle() {
    return m_angle;
}

/**
 * @brief Return the intercept, or when the line crosses the y axis
 * 
 * @return float Return the intercept
 */
float Line::getIntercept() {
    return m_intercept;
}

/**
 * @brief Return the slope, or rise over run, of the line
 * 
 * @return float Return the slope of the line
 */
float Line::getSlope() {
    return m_slope;
}

/**
 * @brief Return the first point used in the creation of the line object. Does not
 * necessarily need to be before (in either x or y) the second point. Returned point
 * does have any offset effects applied to it.
 * 
 * @return sf::Vector2f The first point (x, y)
 */
sf::Vector2f Line::getStart() {
    return m_start;
}

/**
 * @brief Return the second point used in the creation of the line object. Does not
 * necessarily need to be after (in either x or y) the first point. Returned point
 * does have any offset effects applied to it.
 * 
 * @return sf::Vector2f The second point (x, y)
 */
sf::Vector2f Line::getEnd() {
    return m_end;
}

/**
 * @depracated
 * 
 * @brief This method was created for the sole purpose of dubugging line intersections, and should never really be used to draw a line
 * It is for this reason that we don't hold the rectangle shape as a member variable, because once this issue has been solved,
 * we should never need this again.
 * 
 * @param color The color to draw the line as
 * @return RectangleShape* A drawable rectangle that represents our line
 */
sf::RectangleShape* Line::getDrawable(sf::Color color) {
    sf::RectangleShape* r = new sf::RectangleShape();

    r->setSize(sf::Vector2f(sqrt(pow(m_start.x - m_end.x, 2) + pow(m_start.y - m_end.y, 2)), 2));
    r->setOrigin(r->getGlobalBounds().width / 2, r->getGlobalBounds().height / 2 );
    r->setRotation(m_angle);
    r->setFillColor(color);
    r->setPosition((m_start + m_end) / 2.0f);

    return r;
}

/**
 * @brief Get a vector that represents the perpendicular to the line, always has a
 * magnitude of 1
 * 
 * @return sf::Vector2f A vector in the perpendicular direction to the line, with a magnitude of 1
 */
sf::Vector2f Line::getPerpendicular() {
    // Take the negative reciprical of the slope
    float pSlope = -1 / getSlope();

    // Now our slope is y/x, so our vector is (1, slope)
    sf::Vector2f perpendicular(1, pSlope);
    
    // And normalize it
    VectorMath::normalize(perpendicular);

    return perpendicular;
}