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

void VectorMath::normalize(Vector2f& v) {
    float denom = 1 / sqrt(v.x*v.x + v.y*v.y);
    v.x *= denom;
    v.y *= denom;
}