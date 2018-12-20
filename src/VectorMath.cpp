#include "VectorMath.hpp"

float VectorMath::dot(Vector2f v1, Vector2f v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float VectorMath::dot(Vector3f v1, Vector3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}