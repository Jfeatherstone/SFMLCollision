#include "VectorMath.hpp"

/**
 * @brief Compute the dot product between two vectors (sf::sf::Vector2f)
 * 
 * @param v1 The first vector
 * @param v2 The second vector
 * @param value The variable in which the value will be stored in
 */
float VectorMath::dot(sf::Vector2f v1, sf::Vector2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}


/**
 * @brief Compute the dot product between two vectors (sf::sf::Vector3f)
 * 
 * @param v1 The first vector
 * @param v2 The second vector
 * @param value The variable in which the value will be stored in
 */
float VectorMath::dot(sf::Vector3f v1, sf::Vector3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float VectorMath::cross(sf::Vector2f v1, sf::Vector2f v2) {
    return VectorMath::mag(v1) * VectorMath::mag(v2) * sin(VectorMath::angleBetween(v1, v2));
}

/**
 * @brief Find the angle between two vectors (sf::sf::Vector2f) in 2D
 * 
 * @param v1 The first vector
 * @param v2 The second vector
 * @param angle The variable in which the angle will be stored in
 */
float VectorMath::angleBetween(sf::Vector2f v1, sf::Vector2f v2) {
    float dot = VectorMath::dot(v1, v2);
    float mag1 = sqrt(pow(v1.x, 2) + pow(v1.y, 2));
    float mag2 = sqrt(pow(v2.x, 2) + pow(v2.y, 2));
    return acos(dot / (mag1 * mag2));
    
}

/**
 * @brief Apply a rotation transformation to a point about some origin
 * 
 * @param p The point we want to rotate. Rotated value will overwrite previous value
 * @param origin The point we are rotating p about
 * @param angleInDegrees The amount we are rotating, in degrees
 */
void VectorMath::rotate(sf::Vector2f& p, sf::Vector2f origin, float angleInDegrees) {

    angleInDegrees *= M_PI / 180.0f; // Convert to radians

    p -= origin;

    sf::Vector2f pp;
    pp.x = p.x * cos(angleInDegrees) - p.y * sin(angleInDegrees);
    pp.y = p.x * sin(angleInDegrees) + p.y * cos(angleInDegrees);

    p = pp + origin;
}

/**
 * @brief Find the magnitude of a vector (sf::sf::Vector2f)
 * 
 * @param v The vector
 * @return float The magnitude
 */
float VectorMath::mag(sf::Vector2f v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

/**
 * @brief Adjust a vector (sf::sf::Vector2f) such that its total magnitude is equal to the
 * parameter provided
 * 
 * @param v The vector. The normalized value will overwrite the old value
 * @param magnitude The magnitude we want to the vector to have
 */
void VectorMath::normalize(sf::Vector2f& v, float magnitude) {
    float denom = magnitude / sqrt(v.x*v.x + v.y*v.y);
    v.x *= denom;
    v.y *= denom;
}

/**
 * @brief Find what quadrant a point is in relative to a origin
 * 
 * @param point The point who quadrant we want to find
 * @param origin The origin
 * @return int A quadrant number (1-4)
 */
int VectorMath::quadrant(sf::Vector2f point, sf::Vector2f origin) {
    // First we want to account for the origin
    point -= origin;

    /*
    A note about the equals below:
    We assume that if a point is on any of the axes, it will be counted as a part of the upper quadrant.
    This means that a point on the x axis with a positive x will be in quadrant 1 and if it has a negative x
    it will be in the 2nd quadrant
    */
    if (point.x >= 0 && point.y >= 0)
        return 1;
    if (point .x <= 0 && point.y >= 0)
        return 2;
    if (point.x < 0 && point.y < 0)
        return 3;
    return 4;
}