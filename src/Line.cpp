#include "Line.hpp"

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

    m_start = p1;
    m_end = p2;

    calculateSlope();
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
void Line::calculateSlope() {
    m_slope = (m_end.y - m_start.y) / (m_end.x - m_start.x);
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
void Line::calculateIntercept() {
    m_intercept = m_start.y - m_slope*m_start.x;
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
void Line::calculateAngle() {
    // We act as though the start point is our origin (0, 0) by subtracting it from the first
    sf::Vector2f v = m_end - m_start;
    // And now we calculate the angle, be sure to note that it is converted to degrees
    m_angle = atan(v.y / v.x) * 180.0f /M_PI;
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
 * necessarily need to be before (in either x or y) the second point.
 * 
 * @return sf::Vector2f The first point (x, y)
 */
sf::Vector2f Line::getStart() {
    return m_start;
}

/**
 * @brief Return the second point used in the creation of the line object. Does not
 * necessarily need to be after (in either x or y) the first point.
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