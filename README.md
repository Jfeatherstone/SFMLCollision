# SFML Collision

This project is an extension of a previous repo ([SFMLHitbox](https://github.com/Jfeatherstone/SFMLHitbox)) where the goal is to create an easily implemented object that allows for accurate hitbox detection on both convex and concave shapes. By default, SFML only has a ConvexShape, for which there is no intersects function.


The user will be able to use the Polygon shape in two different ways:

    - By providing a texture to the constructor which will identify important pixels to be included as verticies and create the shape based on these. This will also allow for the user to define the level of accuracy as either pixel perfect or approximate.
    - By proving a vector of pre assembled verticies, possibly from some other SFML class by using the getPoints() method.


## Theory

There are several aspects to how this library will work, many of which are complicated and so will be overviewed below.


### Barycentric Coordinates

The intersection algorithm will be based around the idea of barycentric coordinates, which center around a triangle. Every triangle has three verticies, and thus three sides. This coordinate system consists of three values that represent the point's relationship to each individual vertex. If all three of the barycentric are positive, it means that the point is located inside of the triangle. The figure below shows an example of what values you would get for different point locations relative to a given triangle.

![diagram1](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/diagram1.png)


This will be implemented in our Polygon class, along with several other algorithms to reduce the amount of computations per collision detection.

### Triangle Splitting

To properly implement barycentric coordinates in our polygon, we need to divide it into a grouping of triangles, which may seem rather easy given that we have a list of verticies, but there is one small issue. To properly optimize our triangles,
we need to find the centroid of our polygon to act as the central vertex for every triangle. Since we want to be able to model all types of shapes -- convex, concave, regular, irregular, etc. -- we need a consistent way to determine the "center" of the our shape.

The way we are going to do this is by finding the leftmost, rightmost, highest, and lowest, coordinate components and creating a rectangle. We can then take the center of this rectangle as the approximate centroid of our polygon.