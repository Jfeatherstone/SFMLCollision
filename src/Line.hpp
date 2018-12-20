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
#include <iostream>
#include <tgmath.h>

using namespace std; // Standard ns
using namespace sf; // SFML ns

/*
    SCOPE

This line class will be used for two main purposes:
    1. Used for calculations in any subsequent files with the end goal of being able to detect collisions in 2D
    2. Used as a graphical representation in a given SFML window

*/

class Line {

private:

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
    Since we will need points on our line at some point or another, we can add them to a vector
    */
    vector<Vector2f> m_points;

    float calculateSlope(Vector2f p1, Vector2f p2);
    float calculateIntercept();
    void addPoint(Vector2f p);
    float calculateAngle();
    
public:

    /*
    The constructor below will take in two points and find the line between them
    In this sense, it creates a line *segment* as opposed to a real line, but the calculations are not
    different.
    This is what we will use for calculations the majority of the time

    We also will have several other constructors that may not be used as much, but could at some point
    be conveinient
    */
    Line(Vector2f p1, Vector2f p2);
    Line(Vector2f p, float slope);

    /*
    For finding the y value at any given x value
    */
    float y(float x);

    /*
    Some getters
    */
    float getAngle();
    float getIntercept();
    float getSlope();

};