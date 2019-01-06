#include "Line.hpp"

/*
    This value is to denote a vertical line, which would otherwise have a slope of nan, or infinity

The only importance in the value is that no other line should every organically have this slope
*/
float Line::VERTICAL_SLOPE = 1.12312354134;


/*
    CONSTRUCTORS
*/

Line::Line(Vector2f p1, Vector2f p2) {
    // This automatically assigns our slope member variable so we don't need to set it here

    if (p1.x == p2.x)
        m_vertical = true;
    if (p1.y == p2.y)
        m_horizontal = true;

    calculateSlope(p1, p2);
    //if (p1.x <= p2.x) {
        m_start = p1;
        m_end = p2;
    //} else {
    //    m_start = p2;
    //    m_end = p1;
    //}
    calculateQuadrant();
    calculateIntercept();
    calculateAngle();
}

Line::Line() {

}

/*
    SLOPE
    
This is a simple y2-y1/x2-x1 calculation
*/

float Line::calculateSlope(Vector2f p1, Vector2f p2) {
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

float Line::calculateIntercept() {
    m_intercept = m_start.y - m_slope*m_start.x;
    return m_intercept;
}

/*
    QUADRANT
*/
int Line::calculateQuadrant() {
    Vector2f v = m_end - m_start;
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
            m_angle = 270;
        else
            m_angle = 90;
        
        return m_angle;
    }
    
    /*
    This next part accounts for any angle that isn't an integer multiple of PI/2.

    Depending on what quadrant our line is in, we have to do a few adjustments to the actual angle, since we are
    setting 0 degrees to be on the x-axis in a cartesian plane.
    */

    // We first act as though the start point is our origin (0, 0), and calculate what the second points would be
    Vector2f v = m_end - m_start;

    switch (m_quadrant) {
        case 0:
            m_angle = 90 - atan(abs(v.y/v.x)) * 180 /M_PI;
            break;
        case 1:
            m_angle = 270 + atan(abs(v.y/v.x)) * 180 /M_PI;
            break;
        case 2:
            m_angle = 270 - atan(abs(v.y/v.x)) * 180 /M_PI;
            break;
        case 3:
            m_angle = 90 + atan(abs(v.y/v.x)) * 180 /M_PI;
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

float Line::y(float x) {
    return m_slope*x + m_intercept;
}

/*
    INTERSECTS
*/
bool Line::intersects(Line line, Vector2f& intersectionPoint) {
    // Make sure the intersection is actually capable of happening based on the bounds of each line
    intersectionPoint.x = -1;
    intersectionPoint.y = -1;

    //if (line.getStart().x > getEnd().x || line.getEnd().x < getStart().x) {
        //return false;
    //}


    // Parallel lines
    if (line.getSlope() == getSlope()) {
        if (line.getIntercept() == getIntercept()
        && (line.getStart().x <= getEnd().x || line.getEnd().x >= getStart().x)) {
        intersectionPoint.x = line.getStart().x;
        intersectionPoint.y = line.getStart().y;      
            return true;
        } else
            return false;
    }

    // Otherwise, we set the two y equations equal to each other and solve for x
    intersectionPoint.x = (getIntercept() - line.getIntercept()) / (line.getSlope() - getSlope());
    intersectionPoint.y = y(intersectionPoint.x);

    // Make sure the point is between the end and start of both lines
    if ((intersectionPoint.x >= m_start.x && intersectionPoint.x <= m_end.x)
    && (intersectionPoint.x >= line.getStart().x && intersectionPoint.x <= line.getEnd().x)
    && ((intersectionPoint.y >= m_start.y && intersectionPoint.y <= m_end.y)
    || (intersectionPoint.y <= m_start.y && intersectionPoint.y >= m_end.y))
    && ((intersectionPoint.y >= line.getStart().y && intersectionPoint.y <= line.getEnd().y)
    || (intersectionPoint.y <= line.getStart().y && intersectionPoint.y >= line.getEnd().y)))
        return true;

    intersectionPoint.x = -1;
    intersectionPoint.y = -1;
    return false;
}

bool Line::intersects(Line line) {
    Vector2f v;
    return intersects(line, v);
}

/*
    OFFSET

Since we will likely need to offset our lines because our lines will not always be at (0, 0), we have a method to 
properly store and change the offset.

We store this value as a member variable, so that if we need to offset again, we don't have to account for the previous offset
For example:

someLine.offset(Vector2f(10, 10));
// The origin of our coordinate system will be set at (10, 10)

someLine.offset(Vector2f(20, 20));
// The origin of our coordinate system will be set at (20, 20), NOT (30, 30)
*/
void Line::offset(Vector2f offset) {
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

float Line::getAngle() {
    return m_angle;
}

float Line::getIntercept() {
    return m_intercept;
}

float Line::getSlope() {
    return m_slope;
}

Vector2f Line::getStart() {
    return m_start;
}

Vector2f Line::getEnd() {
    return m_end;
}

/*
    GET DRAWABLE

This method was created for the sole purpose of dubugging line intersections, and should never really be used to draw a line
It is for this reason that we don't hold the rectangle shape as a member variable, because once this issue has been solved,
we should never need this again.
*/
RectangleShape* Line::getDrawable() {
    RectangleShape* r = new RectangleShape();

    r->setSize(Vector2f(sqrt(pow(m_start.x - m_end.x, 2) + pow(m_start.y - m_end.y, 2)), 2));
    r->setOrigin(r->getGlobalBounds().width / 2, r->getGlobalBounds().height / 2 );
    r->setRotation(m_angle);
    r->setFillColor(Color::Magenta);
    r->setPosition((m_start + m_end) / 2.0f);

    return r;
}