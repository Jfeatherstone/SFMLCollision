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

/**
 * @brief This class contains several different mathematical operations and calculations used
 * to apply transformations, find dot/cross products, and several other use cases. All methods
 * defined in this class are static (and any new additions should follow suit), though there is
 * no other connection between the functions. Also, not all functions use vectors, as the name
 * might suggest, but rather that they have to deal with directionality.
 * 
 * Dependencies:
 * <SFML/Graphics.hpp> (sf::Vector2f)
 * <tgmath.h> (cos and sin)
 * 
 */
class VectorMath {

public:

    /**
     * @brief Compute the dot product between two vectors (sf::Vector2f). The order of the
     * vectors given doesn't matter.
     * 
     * @param v1 The first vector
     * @param v2 The second vector
     * @param value The variable in which the value will be stored in
     */
    static float dot(sf::Vector2f v1, sf::Vector2f v2);

    /**
     * @brief Gives the magnitude of the crossed vector between two 2d vectors (sf::Vector2f).
     * The order of the vectors given doesn't matter, as we are only looking at magnitude.
     * 
     * @param v1 The first vector
     * @param v2 The second vector
     * @return float 
     */
    static float cross(sf::Vector2f v1, sf::Vector2f v2);

    /**
     * @brief Find the angle between two vectors (sf::Vector2f) in 2D
     * 
     * @param v1 The first vector
     * @param v2 The second vector
     * @param angle The variable in which the angle will be stored in
     */
    static float angleBetween(sf::Vector2f v1, sf::Vector2f v2);

    /**
     * @brief Apply a rotation transformation to a point about some origin
     * 
     * @param p The point we want to rotate. Rotated value will overwrite previous value
     * @param origin The point we are rotating p about
     * @param angleInDegrees The amount we are rotating, in degrees
     */
    static void rotate(sf::Vector2f& p, sf::Vector2f origin, float angle);

    /**
     * @brief Find the magnitude of a vector (sf::Vector2f)
     * 
     * @param v The vector
     * @return float The magnitude of the given vector
     */
    static float mag(sf::Vector2f v);

    /**
     * @brief Adjust a vector (sf::Vector2f) such that its total magnitude is equal to the
     * parameter provided
     * 
     * @param v The vector. The normalized value will overwrite the old value
     * @param magnitude The magnitude we want to the vector to have, defualt is 1
     */
    static sf::Vector2f normalize(sf::Vector2f v, float magnitude = 1);

};