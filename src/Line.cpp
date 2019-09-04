#include "Line.hpp"

/*
    CONSTRUCTORS
*/
Line::Line(const sf::Vector2f p1, const sf::Vector2f p2):
        m_start(p1), m_end(p2) {
            
    // This automatically assigns our slope member variable so we don't need to set it here
    calculateSlope();
    calculateIntercept();
    calculateAngle();
}

Line::Line() {

}

/*
    SLOPE
    
This is a simple y2-y1/x2-x1 calculation
*/
void Line::calculateSlope() {
    m_slope = (m_end.y - m_start.y) / (m_end.x - m_start.x);
}

/*
    INTERCEPT

This will use y = mx + b to find the intercept
*/
void Line::calculateIntercept() {
    m_intercept = m_start.y - m_slope*m_start.x;
}

/*
    ANGLE

For this calculate we assume that intercept is 0, i.e. our coordinate origin is situated on top of the
line.

It is also important to note that we convert everything into degrees, because Transofrmable's setRotation() method takes
the angle value in degrees.
*/

void Line::calculateAngle() {
    // We act as though the start point is our origin (0, 0) by subtracting it from the first
    sf::Vector2f v = m_end - m_start;
    // And now we calculate the angle, be sure to note that it is converted to degrees
    m_angle = atan(v.y / v.x) * 180.0f /M_PI;
}

void Line::calculateNormal() {

    // Check for horiztonal
    float pSlope;
    if (getSlope() == 0)
        pSlope = 1000.0f;
    else
        // Take the negative reciprical of the slope
        pSlope = -1.0f / getSlope();

    // Now our slope is y/x, so our vector is (1, slope)
    sf::Vector2f perpendicular(1, pSlope);
    
    // And normalize it
    perpendicular = VectorMath::normalize(perpendicular);

    m_normal = perpendicular;
}

/*
    Y VALUE

This will give the y value at any given x on the line
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

bool Line::intersects(Line line) {
    sf::Vector2f v;
    return intersects(line, v);
}

/*
    GETTERS
*/

float Line::getAngle() {
    return m_angle;
}

float Line::getIntercept() {
    return m_intercept;
}

float Line::getSlope() {
    return m_slope;
}


sf::Vector2f Line::getStart() {
    return m_start;
}

sf::Vector2f Line::getEnd() {
    return m_end;
}

sf::RectangleShape* Line::getDrawable(sf::Color color) {
    sf::RectangleShape* r = new sf::RectangleShape();

    r->setSize(sf::Vector2f(sqrt(pow(m_start.x - m_end.x, 2) + pow(m_start.y - m_end.y, 2)), 2));
    r->setOrigin(r->getGlobalBounds().width / 2, r->getGlobalBounds().height / 2 );
    r->setRotation(m_angle);
    r->setFillColor(color);
    r->setPosition((m_start + m_end) / 2.0f);

    return r;
}

sf::Vector2f Line::getNormal() {
    if (m_normal == sf::Vector2f(0, 0))
        calculateNormal();
    return m_normal;
}

void Line::setNormal(sf::Vector2f norm) {
    m_normal = norm;
}