#include "Line.hpp"

/*
    CONSTRUCTORS
*/

Line::Line(Vector2f p1, Vector2f p2) {
    // This automatically assigns our member variable so we don't need to set it here
    calculateSlope(p1, p2);
    addPoint(p1);
    addPoint(p2);
    calculateIntercept();
    calculateAngle();
}

Line::Line(Vector2f p, float slope) {
    m_slope = slope;
    addPoint(p);
    calculateIntercept();
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
    // We have to make sure we actually have a point first
    if (m_points.size() > 0) {
        Vector2f p = m_points[0];
        m_intercept = p.y - m_slope*p.x;
    } else
        m_intercept = 0;

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
    float y = m_slope*x + m_intercept;
    // Add the point to our vector
    addPoint(Vector2f(x, y));
    return y;
}

/*
    ADDING POINTS

This will just add a given point that is presumably on our line to the vector of points
*/

void Line::addPoint(Vector2f p) {
    m_points.resize(m_points.size() + 1);
    m_points.push_back(p);
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