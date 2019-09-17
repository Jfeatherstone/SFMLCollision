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
 * @brief A generic force object, used to calculate the impulse and torque applied by a force,
 * which is then used to adjust the linear and angular velocity of the object.
 */
struct Force {
    /**
     * @brief The vector from the point where the force acts to the center of mass of the object
     */
    sf::Vector2f COMVector;

    /**
     * @brief The unit vector in the direction of the force (magnitude of 1)
     */
    sf::Vector2f unitVector;

    /**
     * @brief The time the force is acting for
     */
    float impulseTime;

    /**
     * @brief The magnitude of the force acting
     */
    float magnitude;

    /**
     * @brief Construct a new Force object
     * 
     * @param unit The unit vector in the direction of the force
     * @param time How long the force is acting on the object
     * @param mag The magnitude of the force
     * @param COMVec The vector to the center of mass of the object from the force point, defaults to (0, 0) ie. no torque is applied
     */
    Force(sf::Vector2f unit, float time, float mag, sf::Vector2f COMVec = sf::Vector2f(0, 0)):
     unitVector(unit), impulseTime(time), magnitude(mag), COMVector(COMVec) {

    }

    /**
     * @brief Construct a new Force object
     * 
     * @param forceVector The vector of the force, NOT a unit vector
     * @param time How long the force is acting on the object
     * @param COMVec The vector to the center of mass of the object from the force point, defaults to (0, 0) ie. no torque is applied
     */
    Force(sf::Vector2f forceVector, float time, sf::Vector2f COMVec = sf::Vector2f(0, 0)):
            impulseTime(time),
            magnitude(VectorMath::mag(forceVector)),
            unitVector(VectorMath::normalize(forceVector)),
            COMVector(COMVec) {

    }
};

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
    int m_numVertices;

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
     * @brief This variable is used in deciding how to adjust the normals (ie. if there is an includedPixels arr)
     */
    bool m_wasGeneratedFromImage = false;

    /**
     * @brief 
     * 
     */
    std::vector<std::vector<int>> m_includedPixels;

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
     * @brief Whether the shape can be moved linearly by collisions with other polygons. Doesn't affect movement
     * by setting the velocity of the shape. Default is true.
     */
    bool m_linearFreedom = true;

    /**
     * @brief Whether the shape can be rotated by collisions with other polygons. Doesn't affect movement
     * by setting the angular velocity of the shape. Default is true.
     */
    bool m_rotationalFreedom = true;

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
     * @brief A list of forces that are currently acting on the object, and will be applied and cleared
     * in the next call of the update() method
     */
    std::vector<Force> m_forces;
    
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

    /**
     * @brief Construct a new Polygon object from a given texture (image).
     * 
     * @param texture The texture for the shape/sprite we want to model
     * @param detail The level of detail to keep in the shape, from least to most: Less, More, Optimal, Exact
     * @param ignoredsf::Colors By default, all pixels that arent (0, 0, 0, 0) will be included, any colors specified here will also be ignored
     */
    Polygon(sf::Texture* texture, Detail detail = Detail::Optimal, std::vector<sf::Color> ignoredColors = {});

    /**
     * @brief Construct a new Polygon object from a sf::CircleShape object
     * 
     * @param shape The CircleShape object whose points will be used
     */
    Polygon(sf::CircleShape shape);

    /**
     * @brief Construct a new Polygon object from a sf::RectangleShape object
     * 
     * @param shape The RectangleShape object whose points will be used
     */
    Polygon(sf::RectangleShape shape);

    /**
     * @brief Construct a new Polygon object from a sf::ConvexShape object
     * 
     * @param shape The ConvexShape object who points will be used
     */
    Polygon(sf::ConvexShape shape);


    ///////////////////////////////////////
    //        VERTEX INFO
    ///////////////////////////////////////

    /**
     * @brief Get the number of vertices on our polygon. Overridden from sf::Shape
     * 
     * @return size_t The number of vertices
     */
    virtual size_t getPointCount() const;

    /**
     * @brief Get the vertex at index in the std::vector m_points. Overridden from sf::Shape
     * 
     * @param index The index of the point we are looking for
     * @return sf::Vector2f The point at index in m_points
     */
    virtual sf::Vector2f getPoint(size_t index) const;


    /**
     * @brief Returns the entire std::vector of points that represent the shape, without any modifications from
     * transformations (rotate, move, scale)
     * 
     * @return std::vector<sf::Vector2f> Our shape's std::vector of vertices
     */
    std::vector<sf::Vector2f> getPoints();

    /**
     * @brief Return the lines that represent the polygon's outline/border
     * 
     * @return std::vector<Line> A std::vector of lines that represent the outline
     */
    std::vector<Line> getLines();

    /**
     * @brief Returns the centroid of the shape (does not recalculate it)
     * 
     * @return sf::Vector2f The centroid of the shape
     */
    sf::Vector2f getCentroid();

    ///////////////////////////////////////
    //        PHYSICAL PROPERTIES
    ///////////////////////////////////////

    /**
     * @brief Set whether the shape is solid (can collide with other shapes)
     * 
     * @param state Whether or not the shape is solid
     */
    void setSolid(bool state);

    /**
     * @brief Check whether or not the shape can collide with other shapes
     * 
     * @return true Can collide
     * @return false Cannot collide
     */
    bool isSolid();

    /**
     * @brief Set whether the shape can be moved by being collided with by another object
     * 
     * @param value Whether or not the shape can be moved by another polygon
     */
    void setDegreesOfFreedom(bool canBeMovedLinearly, bool canBeRotated);

    /**
     * @brief Get whether the shape can be moved linearly by being collided with by another object
     * 
     * @return true The shape can be moved
     * @return false The shape cannot be moved
     */
    bool getLinearFreedom();

    /**
     * @brief Get whether the shape can be rotated by being collided with by another object
     * 
     * @return true The shape can be rotated
     * @return false The shape cannot be rotated
     */
    bool getRotationalFreedom();

    /**
     * @brief Set the density of the object, used in calculate its mass and moment of inertia (default is 1)
     * and recalculate both values
     * 
     * @param newDensity The density of the object (default is 1)
     */
    void setDensity(float newDensity);

    /**
     * @brief Get the relative density of the polygon
     * 
     * @return float The density of the polygon
     */
    float getDensity();

    /**
     * @brief Return the mass of the polygon, using the density and area to calculate
     * 
     * @return float The mass of the shape
     */
    float getMass();

    /**
     * @brief Return the moment of inertia of the polygon, using the density and vertex distribution
     * 
     * @return float The moment of inertia of the shape
     */    
    float getMomentOfInertia();
    
    /**
     * @brief Get the Center Of Mass object
     * 
     * @return sf::Vector2f The local coordinates of the center of mass of the object
     */
    sf::Vector2f getCenterOfMass();

    /**
     * @brief This will return the parameter that describes how the polygon responds to forces
     * and how it bends and distorts
     * 
     * @return float The Young's Modulus of the shape, default is 1
     */
    float getYoungsModulus();

    /**
     * @brief Change the parameter that describes how the polygon responds to forces and how it
     * bends and distorts
     * 
     * @param youngsModulus The new value for the young's modulus, default is 1
     */
    void setYoungsModulus(float youngsModulus);

    /**
     * @brief Get the Gamma object
     * 
     * @return float Return the gamma physical property of the object
     */
    float getGamma();

    /**
     * @brief Set the Gamma physical property
     * 
     * @param gamma The new gamma physical property value
     */
    void setGamma(float gamma);

    /**
     * @brief This is a static method that finds the area of any given shape (std::vector of points)
     * Ngl, I don't remember where I found this method for finding the area of a polygon, but 
     * will post when I find it. 
     * 
     * @param points A Vector of points the represent our shape. See Polygon::getPoints()
     * @param value A referenced float that our area will be stored in
     */
    static void calculateArea(std::vector<sf::Vector2f> points, float& value);

    /**
     * @brief Get the Area object
     * 
     * @return float The area of the polygon
     */
    float getArea();

    ///////////////////////////////////////
    //              MOTION
    ///////////////////////////////////////

    void addForce(Force force);
    void applyForces();

    void setVelocity(sf::Vector2f newVelocity);
    sf::Vector2f getVelocity();
    void setAngularVelocity(float newAngularVelocity);
    float getAngularVelocity();

    void update(float elapsedTime);

    ///////////////////////////////////////
    //          TRANSFORMATIONS
    ///////////////////////////////////////

    /*
    These methods a pseudo-overriden in that they reference their super class
    counterpart, though with an extra step (creating our lines) afterwards

    This makes it such that we don't need to recreate our lines every time we fetch them, only
    when we change things using the mehtods below.

    There is likely a better way to do this, but I'm not aware of it right now
    */

    /**
     * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param scale The scaling factors for our polygon
     */
    void setScale(const sf::Vector2f& scale);

    /**
     * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param xFactor The x scaling factor
     * @param yFactor The y scaling factor
     */
    void setScale(float xScale, float yScale);

    /**
     * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param scale The scaling factors for our polygon
     */
    void scale(const sf::Vector2f& scale);

    /**
     * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param xFactor The x scaling factor
     * @param yFactor The y scaling factor
     */
    void scale(float xFactor, float yFactor);

    /**
     * @brief An overriden method from sf::Shape that changes the rotation like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param angle The angle we are setting the rotation to (default is 0)
     */
    void setRotation(float angle);

    /**
     * @brief An overriden method from sf::Shape that changes the rotation like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param angle The angle we are rotating the shape by
     */
    void rotate(float angle);

    /**
     * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param position The new x and y coordinates of the shape
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param x New x coordinate
     * @param y New y coordinate
     */
    void setPosition(float x, float y);

    /**
     * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param d The amount to change x and y by 
     */
    void move(const sf::Vector2f& d);

    /**
     * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
     * and also recreates the lines that represent the shape.
     * 
     * @param dx Amount to change the x coordinate by
     * @param dy Amount to change the y coordinate by
     */
    void move(float dx, float dy);

    /**
     * @brief Set the Size object in pixels, (will scale the object to the proper size)
     * 
     * @param size The new size of the object
     */
    void setSize(sf::Vector2f size);

    /**
     * @brief Set the Size object in pixels, (will scale the object to the proper size)
     * 
     * @param width The new width of the object
     * @param height The new height of the object
     */
    void setSize(float width, float height);

    ///////////////////////////////////////
    //          INTERSECTION
    ///////////////////////////////////////

    /*
    The big boys - 
    The latter three will actually just convert each respective shape into a polygon type and call the first
    intersection function

    It is also important to note that these (due to their scope) are not defined within Polygon.cpp but rather
    Intersects.cpp
    */

    /**
     * @brief Check the intersection between two polygon shapes. Has three levels of detection, to reduce
     * unecessary calculations and resource usage. Returns no information about collision after-effects
     * or resultants. For that, see intersects(Polygon shape, sf::Vector2f& resultant).
     * 
     * @param shape The shape we are checking to be colliding with the current one
     * @return true The two shapes are colliding
     * @return false The two shapes aren't colliding
     */
    bool intersects(Polygon shape);

    /**
     * @brief A wrapper method to check the intersection between a Polygon shape and a RectangleShape
     * by converting it to a polygon and then using our full intersection method
     * See intersects(Polygon shape) for the full intersection method
     * 
     * @param shape The shape we are checking to be colliding with the current one
     * @return true The two shapes are colliding
     * @return false The two shapes aren't colliding
     */
    bool intersects(sf::RectangleShape shape);

    /**
     * @brief A wrapper method to check the intersection between a Polygon shape and a CircleShape
     * by converting it to a polygon and then using our full intersection method
     * See intersects(Polygon shape) for the full intersection method

    * @param shape The shape we are checking to be colliding with the current one
    * @return true The two shapes are colliding
    * @return false The two shapes aren't colliding
    */
    bool intersects(sf::CircleShape shape);

    /**
     * @brief A wrapper method to check the intersection between a Polygon shape and a ConvexShape
     * by converting it to a polygon and then using our full intersection method
     * See intersects(Polygon shape) for the full intersection method
     * 
     * @param shape The shape we are checking to be colliding with the current one
     * @return true The two shapes are colliding
     * @return false The two shapes aren't colliding
     */
    bool intersects(sf::ConvexShape shape);

    /*
    We also may want to know which direction to move the objects after they have collided, so we can do that
    by passing in a reference to a vector
    */
    std::vector<sf::Shape*> intersectAndResolve(Polygon& shape);
    bool intersects(sf::RectangleShape shape, sf::Vector2f& resultant);
    bool intersects(sf::CircleShape shape, sf::Vector2f& resultant);
    bool intersects(sf::ConvexShape shape, sf::Vector2f& resultant);

    ///////////////////////////////////////
    //          CONTAINS
    //     NOT IMPLEMENTED YET (WIP)
    ///////////////////////////////////////

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

};