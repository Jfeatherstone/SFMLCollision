# SFML Collision

This project is an extension of a previous repo ([SFMLHitbox](https://github.com/Jfeatherstone/SFMLHitbox)) where the goal is to create an easily implemented object that allows for accurate hitbox detection on both convex and concave shapes. By default, SFML only has a ConvexShape, for which there is no intersects function.


The user will be able to use the Polygon shape in two different ways:

1. By providing a texture to the constructor which will identify important pixels to be included as verticies and create the shape based on these. This will also allow for the user to define the level of accuracy as either pixel perfect or approximate.
2. By proving a vector of pre assembled verticies, possibly from some other SFML class by using the getPoints() method.


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

### Texture to Vertex Conversion

This isn't so much mathematical as it is programmatic. This section of code can be found in the polygon constructor that takes in a texture, a level of detail, and an optional list of colors.

Note: the notation for images below is as follows
- 0 denotes empty space (will not be included)
- 1 denotes a vertex that will be included
- 2 denotes the inside of a shape that will not be included
- 3 denotes a vertex that was included, but no longer will be


The first operation that is performed isolates the important colors in the image. We divide the image into a vertex of colors and then decide whether or not we would like to include a given pixel in the polygon. By default, any color that isn't (0, 0, 0, 0) will be included, but if a list of ignored colors is provided, anything on that list will be excluded as well.
![step1](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step1.png)

Now that we have a vertex of 1s representing parts to be included and 0s that represent empty space, we begin removing excess points. The most obvious part to remove here is the inside of the shape. To properly represent our image, we really only need to identify the outline, so we remove anything that isn't accessible to an outside shape. In this stage, we also fill in any inside details such that they are removed. An example of this would be a donut, whose empty center would be filled in and then subsequently removed. In this example, both a donut and a cicle would translate to the exact same polygon.
![step2](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step2.png)

The next group of verticies to be removed are those that are in a straight line, of which we really only need to two endpoints of said line. This is really just checking above and below and left and right of a point and removing the intermediate verticies.
![step3](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step3.png)

The last step of identify verticies is very similar to the previous one, but instead works with diagonal lines.
![step4](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step4.png)

These steps remove just about all of the excess verticies we have, and provide a rather good, just-about pixel-perfect representation of our texture.

## References / More Information

1. [Juan José Jiménez, Rafael J. Segura, Francisco R. Feito, Efficient Collision Detection between 2D Polygons, 2004](http://wscg.zcu.cz/wscg2004/Papers_2004_Full/B83.pdf)
2. [Barycentric coordinate calculation](https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates)
3. [General barycentric coordinate info](https://en.wikipedia.org/wiki/Barycentric_coordinate_system)