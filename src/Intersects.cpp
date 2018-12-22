#include "Polygon.cpp"


bool Polygon::intersects(FloatRect shape) {
    // Since there isn't a getPoints() method, we have to do a little work ourselves
    vector<Vector2f> points;
    points.resize(4);
    points[0] = Vector2f(shape.left, shape.top); // Top left
    points[1] = Vector2f(shape.left, shape.top + shape.height); // Bottom left
    points[2] = Vector2f(shape.left + shape.width, shape.top); // Top right
    points[3] = Vector2f(shape.left + shape.width, shape.top + shape.height); // Bottom right

    // Now we just run our other intersection as promised
    Polygon poly(points);
    return intersects(poly);
}

bool Polygon::intersects(CircleShape shape) {
    // Since there is a getPoint() method for this, we don't have to do any math, just add the points to a vector
    vector<Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    Polygon poly(points);
    return intersects(poly);
    
}

/*
Because of the similarity between the convexshape and circleshape classes (having getPoint and getPointCount) we
can have the exact same code work for both
*/
bool Polygon::intersects(ConvexShape shape) {
    vector<Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }
    
    Polygon poly(points);
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
    
}
