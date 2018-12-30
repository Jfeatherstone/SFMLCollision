#include "Polygon.hpp"

/*
    TODO:
If the first 3 methods are called, a polygon with window-based coordinates will be passed
into the last one, which won't happen if the last one is just called on its own. Figure out
a way to make sure that either method will result in the correct decision being made
*/

bool Polygon::intersects(RectangleShape shape) {
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
For these next two, it is important to note that we don't have to do any math on the verticies because
they are given in local coordinates, in relation to 0, 0 being at the top left of the actual shape's rectangle
*/

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
    /*
    We first check to make sure the two polygons are actually capable of intersecting
    */
    float distance = sqrt(pow(getPosition().x - shape.getPosition().x, 2) + pow(getPosition().y - shape.getPosition().y, 2));
    if (distance > getFarthestVertex() + shape.getFarthestVertex())
        return false;
    /*
    The next order of business here is that we need to adjust our triangles to be relative to the window they are
    drawn on
     */
    vector<Triangle> t1 = getTriangles();
    vector<Triangle> t2 = shape.getTriangles();

    FloatRect r = getGlobalBounds();
    for (int i = 0; i < t1.size(); i++) {
        t1[i].offset(Vector2f(r.left, r.top));
    }

    r = shape.getGlobalBounds();
    for (int i = 0; i < t2.size(); i++) {
        t2[i].offset(Vector2f(r.left, r.top));
    }

    // Next up, we want to remove as many triangles as we can, so we find the centroid of our shapes distance from each triangle
    vector<int> trianglesToRemove;
    for (int i = 0; i < t1.size(); i++) {
        if (m_triangleHeights[i] < distance - shape.getFarthestVertex()) {
            trianglesToRemove.push_back(i);
        }
    }

    for (int i = 0; i < trianglesToRemove.size(); i++) {
        t1.erase(t1.begin() + trianglesToRemove[i]);
        //i--;
    }

    trianglesToRemove.clear();
    for (int i = 0; i < t2.size(); i++) {
        if (shape.getTriangleHeights()[i] < distance - getFarthestVertex()) {
            trianglesToRemove.push_back(i);
        }
    }

    for (int i = 0; i < trianglesToRemove.size(); i++) {
        t2.erase(t2.begin() + trianglesToRemove[i]);
        //i--;
    }


    for (int i = 0; i < t2.size(); i++) {
        for (int j = 0; j < getPointCount(); j++) {
            Vector3f bc;
            t2[i].computeBarycentric(getPoints()[j], bc);
            //cout << bc.x << " " << bc.y << " " << bc.z << endl;
            if ((bc.x >= 0 && bc.x <= 1) && (bc.y >= 0 && bc.y <= 1) && (bc.z >= 0 && bc.z <= 1))
                return true;
        }
    }

    for (int i = 0; i < t1.size(); i++) {
        for (int j = 0; j < shape.getPointCount(); j++) {
            Vector3f bc;
            t1[i].computeBarycentric(shape.getPoints()[j], bc);
            //cout << bc.x << " " << bc.y << " " << bc.z << endl;
            if ((bc.x >= 0 && bc.x <= 1) && (bc.y >= 0 && bc.y <= 1) && (bc.z >= 0 && bc.z <= 1))
                return true;
        }
    }

    
    return false;
}
