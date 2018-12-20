#include "Triangle.hpp"

Triangle::Triangle(Vector2f v1, Vector2f v2, Vector2f v3) {
    m_verticies.resize(3);

    m_verticies[0] = v1;
    m_verticies[1] = v2;
    m_verticies[2] = v3;

    precomputeBarycentric();
}

void Triangle::precomputeBarycentric() {
    /*
    Create our two modified vectors
    */
    m_v1 = m_verticies[1] - m_verticies[0];
    m_v2 = m_verticies[2] - m_verticies[0];

    /*
    Do some dot products
    */
    m_dot11 = VectorMath::dot(m_v1, m_v1);
    m_dot12 = VectorMath::dot(m_v1, m_v2);
    m_dot22 = VectorMath::dot(m_v2, m_v2);

    /*
    Find the inverse of our denominator so we can easily multiply later on 
    */

    m_invDenom = 1.0f / (m_dot11 * m_dot22 - m_dot12 * m_dot12);
}

Vector3f Triangle::computeBarycentric(Vector2f p) {
    Vector2f v3 = p - m_verticies[0];

    float dot13 = VectorMath::dot(m_v1, v3);
    float dot23 = VectorMath::dot(m_v2, v3);

    Vector3f v;
    v.x = (m_dot22 * dot13 - m_dot12 * dot23) * m_invDenom;
    v.y = (m_dot11 * dot23 - m_dot12 * dot13) * m_invDenom;
    v.z = 1.0f - v.x - v.y;

    return v;
}