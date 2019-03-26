/*
MIT License

Copyright (c) 2019 Jack Featherstone

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
#include <tgmath.h>

using namespace sf;

/**
 * @brief This class contains several different mathematical operations and calculations used
 * to apply transformations, find dot/cross products, and several other use cases. All methods
 * defined in this class are static (and any new additions should follow suit), though there is
 * no other connection between the functions. Also, not all functions use vectors, as the name
 * might suggest, but rather that they have to deal with directionality.
 * 
 * Dependencies:
 * <SFML/Graphics.hpp> (Vector2f and Vector3f)
 * <tgmath.h> (cos and sin)
 * 
 * Namespaces:
 * sf (SFML)
 */
class VectorMath {

public:

    static void dot(Vector2f v1, Vector2f v2, float& value);
    static void dot(Vector3f v1, Vector3f v2, float& value);

    static Vector2f cross(Vector2f, Vector2f);
    static Vector3f cross(Vector3f, Vector3f);

    static void angleBetween(Vector2f v1, Vector2f v2, float& angle);

    static void rotate(Vector2f& p, Vector2f origin, float angle);

    static float mag(Vector2f v);

    static void normalize(Vector2f& v, float magnitude = 1);

    static int quadrant(Vector2f point, Vector2f origin = Vector2f(0, 0));

};