#include "VectorMath.hpp"

void VectorMath::dot(Vector2f v1, Vector2f v2, float& value) {
    value = v1.x * v2.x + v1.y * v2.y;
}

void VectorMath::dot(Vector3f v1, Vector3f v2, float& value) {
    value = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void VectorMath::angleBetween(Vector2f v1, Vector2f v2, float& angle) {
    float dot;
    VectorMath::dot(v1, v2, dot);
    float mag1 = sqrt(pow(v1.x, 2) + pow(v1.y, 2));
    float mag2 = sqrt(pow(v2.x, 2) + pow(v2.y, 2));
    angle = acos(dot / (mag1 * mag2));
    
}

void VectorMath::rotate(Vector2f& p, Vector2f origin, float angleInDegrees) {

    angleInDegrees *= M_PI / 180.0f; // Convert to radians

    p -= origin;

    Vector2f pp;
    pp.x = p.x * cos(angleInDegrees) - p.y * sin(angleInDegrees);
    pp.y = p.x * sin(angleInDegrees) + p.y * cos(angleInDegrees);

    p = pp + origin;
}

float VectorMath::mag(Vector2f v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

void VectorMath::normalize(Vector2f& v, float magnitude) {
    float denom = magnitude / sqrt(v.x*v.x + v.y*v.y);
    v.x *= denom;
    v.y *= denom;
}

int VectorMath::quadrant(Vector2f point, Vector2f origin) {
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