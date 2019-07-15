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

#include "Line.hpp"
#include "VectorMath.hpp"

/**
 * @brief We can assume that the polygon class will be used for a variety of different objects,
 * some of which will require more accurate and representative shapes than others. Because of
 * this, we will give the user the option to specifiy whether or not to simplify the polygon by
 * a certain degree on its creation.
 * 
 * Note: This option is only present and important in the creation of polygons through a texture.
 * If instead a vector of vertices is provided, this enum is irrelevant, which is why our class
 * doesn't have a member variable to store the level of detail,  since it is only used in the
 * constructor.
 */
enum class Detail {Less, More, Optimal, Exact};

/**
 * @brief The polygon object is the most important aspect of our collisions class, and accounts for most
 * of what will likely be used externally. Polygons can be created through either a texture, a vector of points,
 * or any other child class of sf::Shape (sf::CircleShape, sf::RectangleShape, etc.). 
 * Polygon objects act very similarly to the other sf::Shape classes, being able to be drawn by a RenderWindow 
 * and are able to be transformed by any method from sf::Transformable. On top of these stock attributes, Polygons
 * are also able to be either convex or concave (unlike sf::ConvexShape) and have a method to detect collision between 
 * two instances of the class, or between the class and any other sf::Shape class.
 * With this ability comes a few more paramters that can be changed about the shape, including the density, rigidity,
 * moment of inertia, etc. that all affect how a shape reacts to colliding with another shape.
 * 
 * Dependencies:
 * <SFML/Graphics.hpp>
 * <iostream>
 * <tgmath.h>
 * 
 */
class Polygon: public sf::Shape {

private:

    /**
     * @brief The default density of a given polygon. To change the density from this value,
     * see setDensity(). Value is 1.0f
     */
    const static float DEFAULT_DENSITY;

    /**
     * @brief The default minimum value for the velocity, below which, it will be set to 0. This
     * is done to prevent wandering of polygons very slowly when they should be stopped.
     */
    const static float VELOCITY_THRESHOLD;

    /**
     * @brief The std::vector of points that describe our shape, ordered in either a clockwise or
     * counter-clockwise fashion (no difference betweent the two, but the order must be one or the
     * other, otherwise the shape will look zig-zaggy).
     */
    std::vector<sf::Vector2f> m_points;

    /**
     * @brief The number of vertices the shape has
     */
    int m_numvertices;

    /**
     * @brief The local coordinates of the centroid of the shape. This is calculated by taking the bounding
     * rectangle and taking its center.
     */
    sf::Vector2f m_centroid;

    /**
     * @brief The area of the shape, used in finding the moment of inertia and other such physical properties
     */
    float m_area;

    /**
     * @brief A std::vector of lines that represent the outside of the shape, used to detect collisions between shapes
     */
    std::vector<Line> m_lines;

    /**
     * @brief Whether or not the lines need to be updated to account for a transformation, including rotation,
     * translation, scale adjustment, etc.
     */
    bool m_lineUpdateRequired = false;
    
    /**
     * @brief Whether the shape is considered to be solid or not used to determine whether a shape
     * inside of another counts as intersecting. Default is true.
     */
    bool m_isSolid = true;

    /**
     * @brief Whether the shape can be moved by collisions with other polygons. Doesn't affect movement
     * by setting the velocity of the shape. Default is true.
     */
    bool m_moveableByCollision = true;

    /**
     * @brief The density of the shape, used in calculating the moment of inertia and mass of the shape.
     * For default value, see DEFAULT_DENSITY.
     */
    float m_density = DEFAULT_DENSITY;

    /**
     * @brief The mass of the shape, found by multiplying the density times the area.
     */
    float m_mass;

    /**
     * @brief The momemt of inertia of the shape about its center of mass; describes how the object rotates.
     */
    float m_momentOfInertia;

    /**
     * @brief The center of mass of the object found by averaging the points of the shape.
     */
    sf::Vector2f m_centerOfMass;
    
    /**
     * @brief The young's modulus of the shape; describes the elasticity of collisions
     */
    float m_youngsModulus = 1.0f;

    /**
     * @brief No idea what this does, but it describes some physical property of the shape
     */
    float m_gamma = 1.0f;

    /**
     * @brief WIP
     */
    sf::Vector2f m_force;
    
    // Since this is more or a less a lite physics engine, we need to keep track of the object's
    // velocity

    /**
     * @brief The velocity of the shape, used in moving the object (see update()) and detecting collisions
     */
    sf::Vector2f m_velocity;

    /**
     * @brief The angular velocity of the shape, ie. its rotational velocity. Positive means clockwise rotation,
     * negative means counter-clockwise rotation.
     */
    float m_angularVelocity = 0;

    /**
     * @brief The rigidity of the shape. 1 means it loses no energy (elastic), 0 means it sticks (inelastic).
     * @deprecated Replaced by m_youngsModulus
     */
    float m_rigidity = 1;

    /**
     * @brief Checks whether a color is contained within a std::vector of colors
     * 
     * @param vec A std::vector of colors
     * @param c A specific color
     * @return true If vec contains c
     * @return false If vec doesn't contain c
     */
	bool contains(std::vector<sf::Color>& vec, sf::Color c);

    /**
     * @brief Checks whether a point is contained within a std::vector of points
     * 
     * @param vec A std::vector of points
     * @param point A specific point
     * @return true If vec contains point
     * @return false If vec doesn't contain point
     */
	bool contains(std::vector<sf::Vector2f>& hitboxvertices, sf::Vector2f point);

    /**
     * @brief Calculate the centroid of our object by finding the rightmost, leftmost, topmost, etc. points
     * and taking the average of them all. Should give about the same as taking half of each getGlobalBounds()
     * width and height (for setting the origin).
     */
    void findCentroid();

    /**
     * @brief Using our current m_points, we recreate the lines that represent the boundary of our
     * shape. This is called whenever our shape is transformed (moved, rotated, scaled), and assigns the
     * new set of lines to m_lines.
     */
    void createLines();

    /**
     * @brief Calculate the mass of the polygon using the area and density
     */
    void calculateMass();

    /**
     * @brief Calculate the (relative) moment of inertia of the object by using the distance from the
     * centroid to every vertex on the boundary. Only has value when comparing moment of inertia's between
     * shapes, doesn't give the actual moment of inertia of a real object.
     */
    void calculateMomentOfInertia();

public:

    /*
    The constructor that will parse our shape from a texture

    We have two optional parameters here:

    Detail: This could can be either Less or More (enum is defined above, see there for info) which will define either
    approximate accuracy (less) or pixel perfect accuracy (more)

    vector<Color>: In case we want to ignore certain parts of a sprite, we can provide their rgb values
    and the parser will pass over them
    */
    Polygon(sf::Texture* texture, Detail detail = Detail::Optimal, std::vector<sf::Color> ignoredColors = {});

    /*
    Converting other SFML objects into a Polygon type
    */
    Polygon(std::vector<sf::Vector2f> points);
    Polygon(sf::CircleShape shape);
    Polygon(sf::RectangleShape shape);
    Polygon(sf::ConvexShape shape);

    /*
    The following two methods are overridden from Shape
    */
    virtual size_t getPointCount() const;
    virtual sf::Vector2f getPoint(size_t index) const;

    std::vector<sf::Vector2f> getPoints();

    std::vector<Line> getLines();
    sf::Vector2f getCentroid();

    // Values that adjust how collisions affect the shape
    void setSolid(bool state);
    bool isSolid();

    void setRigidity(float value);
    float getRigidity();

    void setMovableByCollision(bool value);
    bool isMovableByCollision();

    void setDensity(float newDensity);
    float getDensity();

    float getMass();
    float getMomentOfInertia();
    
    sf::Vector2f getCenterOfMass();

    float getYoungsModulus();
    float getGamma();
    void setGamma(float gamma);
    void setYoungsModulus(float youngsModulus);

    sf::Vector2f getForce();
    void setForce(sf::Vector2f force);
    void addForce(sf::Vector2f force);
    /*
    We need to be able to apply a velocity to our shape and iterate it through every frame
    */
    void setVelocity(sf::Vector2f newVelocity);
    sf::Vector2f getVelocity();
    void setAngularVelocity(float newAngularVelocity);
    float getAngularVelocity();

    void update(float elapsedTime);
    void adjustFromForce(sf::Vector2f resultant, sf::Vector2f collisionPoint, Polygon& shape);

    /*
    These methods a pseudo-overriden in that they reference their super class
    counterpart, though with an extra step (creating our lines) afterwards

    This makes it such that we don't need to recreate our lines every time we fetch them, only
    when we change things using the mehtods below.

    There is likely a better way to do this, but I'm not aware of it right now
    */
    void setScale(const sf::Vector2f& scale);
    void setScale(float xScale, float yScale);
    void scale(const sf::Vector2f& scale);
    void scale(float xFactor, float yFactor);
    void setRotation(float angle);
    void rotate(float angle);
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    void move(const sf::Vector2f& offset);
    void move(float xOffset, float yOffset);

    /*
    The big boys
    The latter three will actually just convert each respective shape into a polygon type and call the first
    intersection function

    It is also important to note that these (due to their scope) are not defined within Polygon.cpp but rather
    Intersects.cpp
    */
    std::vector<sf::Vector2u> intersects(Polygon shape);
    bool intersects(sf::RectangleShape shape);
    bool intersects(sf::CircleShape shape);
    bool intersects(sf::ConvexShape shape);

    /*
    We also may want to know which direction to move the objects after they have collided, so we can do that
    by passing in a reference to a vector
    */
    bool intersects(Polygon& shape, sf::Vector2f& resultant);
    bool intersects(sf::RectangleShape shape, sf::Vector2f& resultant);
    bool intersects(sf::CircleShape shape, sf::Vector2f& resultant);
    bool intersects(sf::ConvexShape shape, sf::Vector2f& resultant);

    /*
    Another intersection type utility we want is to check whether another shape is inside of this one.
    We will again create severeal wrappers for the main method so we can use native SFML objects if
    we need to.

    These are also defined in Intersection.cpp
    */
    bool contains(Polygon shape);
    bool contains(sf::RectangleShape shape);
    bool contains(sf::CircleShape shape);
    bool contains(sf::ConvexShape shape);

    /*
    For finding the area of our polygons given either a set of points or an actual polygon
    */
    static void getArea(std::vector<sf::Vector2f> points, float& value);
    float getArea();
};