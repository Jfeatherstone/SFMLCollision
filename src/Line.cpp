#include "Line.hpp"

/*
    This value is to denote a vertical line, which would otherwise have a slope of nan, or infinity

The only importance in the value is that no other line should every organically have this slope, and that
this is an arbitrarily high number, to simulate a vertical line without actually being one
*/
float Line::VERTICAL_SLOPE = 100000;


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

    m_start = p1;
    m_end = p2;

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
    Vector2f v = m_end - m_start;

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

float Line::y(float x) {
    return m_slope*x + m_intercept;
}

/*
    INTERSECTS
*/
bool Line::intersects(Line line, Vector2f& intersectionPoint, bool extendLine) {
    /*
    intersectionPoint.x = -1;
    intersectionPoint.y = -1;

    //if (line.getStart().x > getEnd().x || line.getEnd().x < getStart().x) {
        //return false;
    //}

    // Parallel lines
    if (line.getSlope() == getSlope()) {
        //cout << "Parallel" << endl;
        if (line.getIntercept() == getIntercept()
        && (line.getStart().x <= getEnd().x || line.getEnd().x >= getStart().x)) {
        intersectionPoint.x = line.getStart().x;
        intersectionPoint.y = line.getStart().y;      
            return true;
        } else
            return false;
    }

    
    // We need some special cases for vertical lines
    if (getSlope() == VERTICAL_SLOPE) {
        //if (m_start.x <= std::max(line.getStart().x, line.getEnd().x)
        //&& m_start.x >= std::min(line.getStart().x, line.getStart().x)) {
            intersectionPoint.x = m_start.x;
            intersectionPoint.y = line.y(m_start.x);
            //return true;
        //} else   
            //return false;
    } else if (line.getSlope() == VERTICAL_SLOPE) {
        //if (line.getStart().x <= std::max(getStart().x, getEnd().x)
        //&& line.getStart().x >= std::min(getStart().x, getStart().x)) {
            intersectionPoint.x = line.getStart().x;
            intersectionPoint.y = y(line.getStart().x);
            //return true;
        //} else   
            //return false;
    } else {

        // Otherwise, we set the two y equations equal to each other and solve for x
        intersectionPoint.x = (getIntercept() - line.getIntercept()) / (line.getSlope() - getSlope());
        intersectionPoint.y = y(intersectionPoint.x);
    }
    cout << intersectionPoint.x << " " << intersectionPoint.y << endl;

    // If we are extending the line, we want to only check the bounds for the second line
    if (extendLine && intersectionPoint.x <= std::max(line.getStart().x, line.getEnd().x) && intersectionPoint.x >= std::min(line.getStart().x, line.getEnd().x) &&
        intersectionPoint.x >= std::min(line.getStart().y, line.getEnd().y) && intersectionPoint.y <= std::max(line.getStart().y, line.getEnd().y))
        return true;


    // Make sure the point is between the end and start of both lines
    // We use the max and min function here, since the start point isn't required to 
    // be "first" in the line, they are stored in whatever order the user enters them
    if ((intersectionPoint.x <= std::max(m_start.x, m_end.x) && intersectionPoint.x >= std::min(m_start.x, m_end.x) &&
        intersectionPoint.y >= std::min(m_start.y, m_end.y) && intersectionPoint.y <= std::max(m_start.y, m_end.y)) &&
        intersectionPoint.x <= std::max(line.getStart().x, line.getEnd().x) && intersectionPoint.x >= std::min(line.getStart().x, line.getEnd().x) &&
        intersectionPoint.x >= std::min(line.getStart().y, line.getEnd().y) && intersectionPoint.y <= std::max(line.getStart().y, line.getEnd().y))
        return true;

    //intersectionPoint.x = -1;
    //intersectionPoint.y = -1;
    return false;
    */

    // We do this to help with naming and keeping our variables organized
    // from the reference linked above (ik, 1 indexing is gross)
    float x[5] = {0, getStart().x, getEnd().x, line.getStart().x, line.getEnd().x};
    float y[5] = {0, getStart().y, getEnd().y, line.getStart().y, line.getEnd().y};

    // We precompute the denominator to detect parallel lines and to save on resources
    float denominator = ((x[4] - x[3])*(y[2] - y[1]) - (x[2] - x[1])*(y[4] - y[3]));

    // Check for parallel lines
    if (denominator == 0) {
        if (line.getIntercept() == getIntercept())
            return true;
        else
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
RectangleShape* Line::getDrawable(Color color) {
    RectangleShape* r = new RectangleShape();

    r->setSize(Vector2f(sqrt(pow(m_start.x - m_end.x, 2) + pow(m_start.y - m_end.y, 2)), 2));
    r->setOrigin(r->getGlobalBounds().width / 2, r->getGlobalBounds().height / 2 );
    r->setRotation(m_angle);
    r->setFillColor(color);
    r->setPosition((m_start + m_end) / 2.0f);

    return r;
}

/*
    ROTATE
*/

void Line::rotate(Vector2f center, float angle) {
    // First we update our points to be accounted for the center
    Vector2f p1 = m_start - center;
    Vector2f p2 = m_end - center;

    
}