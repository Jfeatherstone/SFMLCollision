/*
MIT License

Copyright (c) 2018 Jack Featherstone

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once
#include <SFML/Graphics.hpp>
#include "Triangle.hpp"
#include <iostream>
#include <tgmath.h>

using namespace sf;
using namespace std;

/*
We can assume that the polygon class will be used for a variety of different objects,
some of which will require more accurate and representative shapes than others. Because of
this, we will give the user the option to specifiy whether or not to simplify the polygon by
a certain degree on its creation.

Note: This option is only present and important in the creation of polygons through a texture.
If instead a vector of verticies is provided, this enum is irrelevant, which is why our class
doesn't have a member variable to store the level of detail,  since it is only used in the
constructor.
*/

enum class Detail {Less, More, Optimal};



class Polygon: public Shape {

private:

    /*
    An array of points that define our shape
    */
    vector<Vector2f> m_points;

    /*
    How many verticies our shape has
    */
    int m_numVerticies;
    Vector2f m_centroid;
    float m_farthestVertex;

    /*
    To detect collision, we will also want to divide our shape into triangles, so we can do that once on creation as to
    not have to repeat it ever again
    */
    vector<Triangle> m_triangles;
    vector<float> m_triangleHeights;
    
	void getPixels();
	bool contains(vector<Color> vec, Color c);
	bool hitboxContainsPoint(vector<Vector2f>& hitboxVerticies, Vector2f point);
    void createTriangles();
    void findCentroid();
    int sign(float value);
public:

    /*
    The constructor that will parse our shape from a texture

    We have two optional parameters here:

    Detail: This could can be either Less or More (enum is defined above, see there for info) which will define either
    approximate accuracy (less) or pixel perfect accuracy (more)

    vector<Color>: In case we want to ignore certain parts of a sprite, we can provide their rgb values
    and the parser will pass over them
    */
    Polygon(Texture* texture, Detail detail = Detail::Less, vector<Color> ignoredColors = {});

    /*
    The following ones are going to be for converting other objects into a polygon
    */
    Polygon(vector<Vector2f> points);
    Polygon(CircleShape shape);
    Polygon(RectangleShape shape);
    Polygon(ConvexShape shape);

    /*
    The following two methods are overridden from Shape
    */
    virtual size_t getPointCount() const;
    virtual Vector2f getPoint(size_t index) const;

    vector<Vector2f> getPoints();

    vector<Triangle> getTriangles();
    float getFarthestVertex();
    vector<float> getTriangleHeights();
    /*
    The big boys
    The latter three will actually just convert each respective shape into a polygon type and call the first
    intersection function

    It is also important to note that these (due to their complexity) are not defined within Polygon.cpp but rather
    Intersects.cpp
    */
    bool intersects(Polygon shape);
    bool intersects(RectangleShape shape);
    bool intersects(CircleShape shape);
    bool intersects(ConvexShape shape);

    /*
    For finding the area of our polygons both given either a set of points or an actual polygon
    */
    static void getArea(vector<Vector2f> points, float& value);
    void getArea(float& value);
};