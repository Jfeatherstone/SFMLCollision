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
#include <iostream>
#include <tgmath.h>
#include "VectorMath.hpp"

/*
    SCOPE

This line class will be used for two main purposes:
    1. Used for calculations in any subsequent files with the end goal of being able to detect collisions in 2D
    2. Used as a graphical representation in a given SFML window

*/

/**
 * @brief The line class is integral to detecting collisions between polygon classes, and thus is used
 * internalling in the intersection method, and Polygon objects. A note about these is that the name is
 * a slight misnomer, in that by default, the lines are actually line segments, having a finite length
 * and a start and end point.
 * 
 * Dependencies:
 * <SFML/Graphics.hpp>
 * <iostream>
 * <tgmath.h>
 * 
 */
class Line {

private:

    static float PARALLEL_LINE_SLOPE_TOLERANCE;
    static float PARALLEL_LINE_DISTANCE_TOLERANCE;

    /*
    The value of our slope
    */
    float m_slope;

    /*
    Our y-intercept value (b) 
    */
    float m_intercept;

    /*
    The angle between our line and the horizontal line x = 0 (x-axis)
    In radians
    This value will only be able to be between pi/2 and -pi/2
    */
    float m_angle;
    


    /*
    Special cases for our lines
    */
    bool m_vertical = false;
    bool m_horizontal = false;
    static float VERTICAL_SLOPE;

    // This is the quadrant the second point would be if the first is at 0,0
    int m_quadrant;

    /*
    Since we will need points on our line at some point or another, we can add them to a vector
    */
    sf::Vector2f m_start;
    sf::Vector2f m_end;

    sf::Vector2f m_offset;

    float calculateSlope(sf::Vector2f p1, sf::Vector2f p2);
    float calculateIntercept();
    float calculateAngle();
    int calculateQuadrant();
    
public:

    /*
    The constructor below will take in two points and find the line between them
    In this sense, it creates a line *segment* as opposed to a real line, but the calculations are not
    different.
    This is what we will use for calculations the majority of the time

    We also will have several other constructors that may not be used as much, but could at some point
    be conveinient
    */
    Line(sf::Vector2f p1, sf::Vector2f p2);
    Line();

    /*
    For finding the y value at any given x value
    */
    float y(float x);
    bool intersects(Line line);
    bool intersects(Line line, sf::Vector2f& intersectionPoint, bool extendLine = false);

    /*
    Some getters
    */
    float getAngle();
    float getIntercept();
    float getSlope();
    sf::Vector2f getStart();
    sf::Vector2f getEnd();

    /*
    This will be used to find the resultant vector of a collision
    */
    sf::Vector2f getPerpendicular();

    void offset(sf::Vector2f offset);

    sf::RectangleShape* getDrawable(sf::Color color = sf::Color::Cyan);
    
    void rotate(sf::Vector2f center, float angle);
};