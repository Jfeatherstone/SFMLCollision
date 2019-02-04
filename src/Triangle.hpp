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
#include "VectorMath.hpp"

using namespace sf;
using namespace std;

/*
The main purpose of this class is to help with collision detection for both concave and convex polygons
Because of this, on the creation of the object, certain values will be precomputed as to save resources
later.

Since it is primarily intended for math and calculations, it will not extend SFML shape (and should thus
never be drawn to the screen) as to keep it lightweight.

For more info on the basis behind this class, see:
https://en.wikipedia.org/wiki/Barycentric_coordinate_system
*/

class Triangle {

private:

    /*
    The three verticies of a triangle
    */
    vector<Vector2f> m_verticies;

    /*
    Some of the precomputed values for our barycentric coordinates

    For more info, see:
    https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    */
    float m_invDenom;

    Vector2f m_v1;
    Vector2f m_v2;

    float m_dot11;
    float m_dot12;
    float m_dot22;
    
    Vector2f m_offset;

    /*
    This will compute a few values that should save resources later

    It is interesting to point out that this main product here, the inverse demoninator for
    barycentric calculations, is stored as the inverse because multiplication is less resource
    intensive than division, and we only divide once as opposed to multiplying everytime we calculate
    these new coordinates.
    */
    void precomputeBarycentric();
public:
    /*
        Our one and (sorta) only constructor  
    
    */
    Triangle(Vector2f v1, Vector2f v2, Vector2f v3);
    
    /*
    This constructor must exist such that we can create a resized vector of triangles, but will never really be used
    */
    Triangle();

    /*
    In case we need to access our verticies
    */
    Vector2f getVertex1();
    Vector2f getVertex2();
    Vector2f getVertex3();

    vector<Vector2f> getVerticies();
    

    /*
    Since our triangles will be moving around the screen (since our objects will be) we will need
    a way to offset their origins to properly represent our shape as far as location goes.
    */
    void offset(Vector2f offset);

    /*
    The meat of the class right here, and its main function
    */
    void computeBarycentric(Vector2f p, Vector3f& result);
};