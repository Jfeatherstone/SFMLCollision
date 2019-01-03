#include "Line.hpp"

/*
    CONSTRUCTORS
*/

Line::Line(Vector2f p1, Vector2f p2) {
    // This automatically assigns our slope member variable so we don't need to set it here
    calculateSlope(p1, p2);
    if (p1.x < p2.x) {
        m_start = p1;
        m_end = p2;
    } else {
        m_start = p2;
        m_end = p1;
    }
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
    ANGLE

Depending on the value for the slope, we calculate the angle differently, based on which quadrant
the line runs through

For this calculate we assume that intercept is 0, i.e. our coordinate origin is situated on top of the
line.
*/

float Line::calculateAngle() {

    m_angle = atan(y(1)/1);
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