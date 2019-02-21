# SFML Collision

This project is an extension of a previous repo ([SFMLHitbox](https://github.com/Jfeatherstone/SFMLHitbox)) where the goal is to create an easily implemented object that allows for accurate hitbox detection on both convex and concave shapes. By default, SFML only has a ConvexShape, for which there is no intersects function.


The user will be able to use the Polygon shape in two different ways:

1. By providing a texture to the constructor which will identify important pixels to be included as verticies and create the shape based on these. This will also allow for the user to define the level of accuracy as either pixel perfect or approximate.
2. By proving a vector of pre assembled verticies, possibly from some other SFML class by using the getPoints() method.


## Theory

There are several aspects to how this library will work, many of which are complicated and so will be overviewed below. This information is not necessary to use the tool, but can be helpful to understand what is going on (especially if you want to contribute to the code).

For information on practical usage of the tool, see the documentation page, which has yet to be created.


### Texture to Vertex Conversion

This isn't so much mathematical as it is programmatic. This section of code can be found in the polygon constructor that takes in a texture, a level of detail, and an optional list of colors.

Note: the notation for images below is as follows
- 0 denotes empty space (will not be included)
- 1 denotes a vertex that will be included
- 2 denotes the inside of a shape that will not be included
- 3 denotes a vertex that was included, but no longer will be as it is not necessary

The following images are generated from this source image:

![source](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/test.png)

The first operation that is performed isolates the important colors in the image. We divide the image into a vertex of colors and then decide whether or not we would like to include a given pixel in the polygon. By default, any color that isn't (0, 0, 0, 0) will be included, but if a list of ignored colors is provided, anything on that list will be excluded as well.
![step1](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step1.jpg)

Now that we have a vertex of 1s representing parts to be included and 0s that represent empty space, we begin removing excess points. The most obvious part to remove here is the inside of the shape. To properly represent our image, we really only need to identify the outline, so we remove anything that isn't accessible to an outside shape. In this stage, we also fill in any inside details such that they are removed. This is done by drawing eight lines in all of the cardinal directions and diagonals, and seeing if they encounter a point that is included. If all eight do, the point must be inside the outline of the object. This can be seen in Images/test6.png and Images/test7.png which will produce the exact same polygon.
![step2](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step2.jpg)

The next group of verticies to be removed are those that are in a straight line, of which we really only need to two endpoints of said line. This is really just checking above and below and left and right of a point and removing the intermediate verticies.
![step3](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step3.jpg)

The next step of identify verticies is very similar to the previous one, but instead works with diagonal lines.
![step4](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step4.jpg)

Finally, we remove any intermediate points along the horizontals, similar to what we did earlier with verticle and horizontal lines.
![step5](https://raw.githubusercontent.com/Jfeatherstone/SFMLCollision/master/Images/step5.jpg)


These steps remove just about all of the excess verticies we have, and provide a rather good, just-about pixel-perfect representation of our texture.

The next step is arguably the hardest, and involves adding our verticies in the proper order. This seems like it should be an easy step, just iterate through our vector and add any points that have a value of 1, but alas. If we were to do this, the order of our verticies would be incorrect, and shape would zig-zag back and forth, and be a terrible representation of the actual shape. Instead, we have to follow one direction, clockwise or counter-clockwise, consistently, which is easier said than done.

One of the saving graces of this process is that due to our vertex reduction, just about every vertex should only be connected to either one two verticies (1), or a place that used to be a vertex (3). The case where this is not true is exmplified in the last image above, where at the top of the shape we have a 2x2 square of all verticies.

We can then keep track of which places (1s or 3s) we have visited, so we don't repeat any values, and each location will only have one other location to go to.

We begin the process by starting at (0, 0) and moving horizontally until we encounter the first pixel marked as a '1'. Once we have this value, we add it to our list of final points, and check the adjacent pixels for either 1s or 3s. The order we check in is clockwise, beginning from the top and ending at the top left.

Depending on whether or not we find a 3 or a 1 changes the next step slightly. In either case, we will move to that pixel and repeat the process of searching adjacent pixels, but will only add the coordinate to our final vector if the value is a 1.

### Vertex Reduction

One of the options that is presented to the user is the level of detail in which to model the texture. There are currently three different levels: Less, More, Optimal. Each of these values represents a percent difference the final polygon is allowed to be from the initial one. No matter the level of detail, the previous conversion is always the same, and this trimming is only done afterwards.

Essentially, we iterate through every points on the shape and find the difference in area if we were to remove it. If we find that the difference is below a certain value, which is based on the level of detail (Less - 15%, More, 7.5%, Optimal - 1%), we actually remove that point and continue. This allows for us to remove insignificant points systematically.

## Intersection Method

### First level detection

The first value that will be check is the least computationally intensive, and is the most broad detection system for our polygons. All we do, is compare the rectangular frames of the polygons and see if these intersect, because if they don't, it is not possible for their contained shapes to. This process does use SFML's stock intersection method between two FloatRects, and so the benchmark comparison between our method and that method should be taken with a grain of salt.

## References / More Information

1. [Juan José Jiménez, Rafael J. Segura, Francisco R. Feito, Efficient Collision Detection between 2D Polygons, 2004](http://wscg.zcu.cz/wscg2004/Papers_2004_Full/B83.pdf)
2. [Barycentric coordinate calculation](https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates)
3. [General barycentric coordinate info](https://en.wikipedia.org/wiki/Barycentric_coordinate_system)
4. [Parametric line intersection method](http://ahinson.com/algorithms_general/Sections/Geometry/ParametricLineIntersection.pdf)