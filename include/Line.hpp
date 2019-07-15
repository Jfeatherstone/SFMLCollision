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

/**
 * @brief The line class is integral to detecting collisions between polygon classes, and thus is used
 * internally in the intersection method, and Polygon object. A note about these is that the name is
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

    /**
     * @brief The slope of the line (rise over run, dy/dx).
     */
    float m_slope;

    /**
     * @brief The intercept of the line (the y value when x = 0). This isn't really used for our
     * polygon calculations, but necessary to define a line.
     */
    float m_intercept;
    
    /**
     * @brief The angle between our line and the horizontal line x = 0 (x-axis) in degrees.
     */
    float m_angle;
    
    /**
     * @brief The beginning point of the line, ie. the one provided first in the constructor. Gives local 
     * coordinates of the point, not its position on the screen (if drawn). Doesn't necessarily have to be
     * spatially "before" the end point.
     */
    sf::Vector2f m_start;

    /**
     * @brief The enbd point of the line, ie. the one provided second in the constructor. Gives local 
     * coordinates of the point, not its position on the screen (if drawn). Doesn't necessarily have to be
     * spatially "after" the beginning point.
     */
    sf::Vector2f m_end;

    /**
     * @brief Calculates the slope of the line, given the two points, m_start and m_end, and stores it
     * in the m_slope variable.
     */
    void calculateSlope();

    /**
     * @brief Calculates the intercept of the line, using the slope and the m_start point, and stores it
     * in the m_intercept variable.
    */
    void calculateIntercept();

    /**
     * @brief Calculates the angle of the line with respect to the horizontal x-axis, using tgmath's atan
     * function, which is converted to degrees (which is what SFML uses) and stored in m_angle
     */
    void calculateAngle();
    
public:

    /*
    The constructor below will take in two points and find the line between them
    In this sense, it creates a line *segment* as opposed to a real line, but the calculations are not
    different.
    This is what we will use for calculations the majority of the time

    We also will have several other constructors that may not be used as much, but could at some point
    be conveinient
    */

    /**
    * @brief Construct a new Line object which spans between two points (that is, it is a line segment).
    * The order of the points provided is irrelevant for the most part (at least externally).
    * 
    * @param p1 The start point
    * @param p2 The end point
    */
    Line(sf::Vector2f p1, sf::Vector2f p2);

    /**
     * @brief Construct a new Line object with no information, used for comparing line objects
     */
    Line();

    /*
    For finding the y value at any given x value
    */

    /**
     * @brief Returns the y value along the line at any given x value
     * 
     * @param x The x value in which the y value is to be known
     * @return float The y value at the given x value
     */
    float y(float x);

    /**
     * @brief Detects whether this line is intersecting with another (within the bounds of both)
     * 
     * @param line The line that may be intersecting with this one
     * @return true The two lines are intersecting
     * @return false The two lines are not intersecting
     */
    bool intersects(Line line);

    /**
     * @brief Detects whether this line is intersecting with another, with a few extra options
     * 
     * @param line The line that may be intersecting with this one
     * @param intersectionPoint A vector that will be assigned to the point in which the two lines
     * intersect. If they do not intersect, it will remain unchanged.
     * @param extendLine If this parameter is set to true, it will extend this line to a full line
     * instead of just a segment, which can be useful in determining whether the line is inside a set
     * of lines (see Polygon::contains)
     * @return true The two lines are intersecting
     * @return false The two lines are not intersecting
     */
    bool intersects(Line line, sf::Vector2f& intersectionPoint, bool extendLine = false);

    /**
     * @brief Get the Angle object
     * 
     * @return float The angle of the line with respect to the x-axis, in degrees
     */
    float getAngle();

    /**
     * @brief Get the Intercept object
     * 
     * @return float The intercept of the line
     */
    float getIntercept();

    /**
     * @brief Get the Slope object
     * 
     * @return float The slope of the line (dy/dx)
     */
    float getSlope();

    /**
     * @brief Get the Start object
     * 
     * @return sf::Vector2f The first point passed to the constructor
     */
    sf::Vector2f getStart();

    /**
     * @brief Get the End object
     * 
     * @return sf::Vector2f The second point passed to the constructor
     */
    sf::Vector2f getEnd();

    /**
     * @brief Get the unit normal vector for the line
     * 
     * @return sf::Vector2f A vector that is perpendicular to the line, with a magnitude of 1
     */
    sf::Vector2f getPerpendicular();

    /**
     * @brief Get a drawable instance (RectangleShape) of the line, such that it can be drawn on a window
     * 
     * @param color The color in which the line will be drawn in, default is sf::Color::Cyan
     * @return sf::RectangleShape* A pointer to the object to be drawn
     */
    sf::RectangleShape* getDrawable(sf::Color color = sf::Color::Cyan);
};