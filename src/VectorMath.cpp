#include "VectorMath.hpp"

float VectorMath::dot(sf::Vector2f v1, sf::Vector2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float VectorMath::cross(sf::Vector2f v1, sf::Vector2f v2) {
    return VectorMath::mag(v1) * VectorMath::mag(v2) * sin(VectorMath::angleBetween(v1, v2));
}

float VectorMath::angleBetween(sf::Vector2f v1, sf::Vector2f v2) {
    float mag1 = mag(v1);
    float mag2 = mag(v2);

    // You can't cross the zero vector
    if (mag1 == 0 || mag2 == 0)
        return 0;

    float dot = VectorMath::dot(v1, v2);
    return acos(dot / (mag1 * mag2));
}

void VectorMath::rotate(sf::Vector2f& p, sf::Vector2f origin, float angleInDegrees) {

    angleInDegrees *= M_PI / 180.0f; // Convert to radians

    p -= origin;

    sf::Vector2f pp;
    pp.x = p.x * cos(angleInDegrees) - p.y * sin(angleInDegrees);
    pp.y = p.x * sin(angleInDegrees) + p.y * cos(angleInDegrees);

    p = pp + origin;
}

float VectorMath::mag(sf::Vector2f v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f VectorMath::normalize(sf::Vector2f v, float magnitude) {
    float denom = magnitude / mag(v);
    v.x *= denom;
    v.y *= denom;
    return v;
}