#include "Polygon.hpp"

/*
    Our parsing (and more lengthy) constructor

This will generate a list of verticies based on the pixels in an image (texture)
*/
Polygon::Polygon(Texture* texture, Detail detail, vector<Color> ignoredColors) {

    /*****************************************
     * 
     * BEGINNING OF SHAPE GENERATION
     * 
     * Beware!
     * The next bit of code is a complete clown fiesta and I recomend scrolling past as fast as you can
     * 
     * This code has been pasted from what used to be several methods but is now just here to avoid
     * the creation of too many member variables.
     * 
     * Another consequence of this is that the old class was solely for hitboxes (which this still kinda
     * is, but not explicitly) so a lot of variables are going to be called hitboxSomething, just a heads up :/
     * 
     * Consider yourself warned...
     * 
     * ******************************************/

    /*****************************************
     *      Parsing RGB values from the image
     * ******************************************/
    vector<Color> pixels;

    // Convert our texture to an image
    Image image = texture->copyToImage();

    // This will be the number of pixels in the image
    int length = image.getSize().x * image.getSize().y;

    //cout << length << endl;
    if (length == 0) {
        // This will happen if our image is empty (the file doesn't exist)
        cout << "Attempt to pass in empty image to constructor!\n In Polygon::Polygon(Texture* texture, Detail detail, vector<Color> ignoredColors)" << endl;
        return;
    }

    // Reassign our pixel vector
    pixels.clear();
    pixels.resize(length);

    // The actual array of pixels
    const Uint8* arr = image.getPixelsPtr();

    // Separate out each color and create an object to add to our vector
    for (int i = 0; i < length; i++) {
        int red = (int) arr[4*i];
        int green = (int) arr[4*i + 1];
        int blue = (int) arr[4*i + 2];
        int alpha = (int) arr[4*i + 3];
        Color c(red, green, blue, alpha);
        pixels.push_back(c);
    }

    /*****************************************
     *      Creating our first set of verticies
     * ******************************************/
    vector<int> hitboxInclude;
    // Next, we want to go through every color and if it isn't empty or on the ignore list, we
    // add it to our include for the hitbox calculation
    hitboxInclude.resize(pixels.size());

    Vector2f textureSize;

    textureSize.x = texture->getSize().x;
    textureSize.y = texture->getSize().y;

    int i = 0;

    for (Color c: pixels) {
        if (!contains(ignoredColors, c) && (int) c.a > 0) {
            //cout << (int)c.r << " " << (int)c.g << " " << (int)c.b << " " << (int)c.a << endl;
            hitboxInclude[i] = 1;
        }
        else
            hitboxInclude[i] = 0;
        i++;
    }

    /*****************************************
     *      Removing the inside
     * ******************************************/
    // Now, we go through and remove everything except for the outline
    vector<int> newHitbox;
    newHitbox.resize(hitboxInclude.size());

    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            // We check if everything surrounding it is a 1 which would denote something inside
            /*
            An explanation of this ungodly if statement:
            To determine whether a pixel is inside of the sprite -- and therefore not needed to determine the
            hitbox -- we have to look at any of the pixels are it.
            The first line of the statement makes sure that the value to the left is included in the hitbox, and
            that it isn't on the end of the previous line (since this isn't actually a 2d array, rather its 1d)

            The second line does the same as the first, but with the right side, checking that the value is 1 and
            that it isn't past the end of the line (onto the next one)

            Next, the line checks that both above and below the pixel are included, which don't need weird
            cases like left and right since they are just up and down

            Lastly, we make sure that we aren't at the bottom line. This was causing an error where some
            on the last line weren't included and I have no idea why, but this seemed like a good way
            to fix it :)
            */
            newHitbox[i*textureSize.x + j] = hitboxInclude[i*textureSize.x + j];

            if ((hitboxInclude[i*textureSize.x + j - 1] == 1 && i*textureSize.x + j - 1 >= i*textureSize.x)
            && (hitboxInclude[i*textureSize.x + j + 1] == 1 && i*textureSize.x + j + 1 < (i+1)*textureSize.x)
            && (hitboxInclude[(i-1)*textureSize.x + j] == 1) && (hitboxInclude[(i+1)*textureSize.x + j] == 1)
            && ((i+1)*textureSize.x + j < hitboxInclude.size()))
            
            newHitbox[i*textureSize.x + j] = 2;
        }
    }
    hitboxInclude = newHitbox;

    /*****************************************
     *      Remove excess verticies
     * ******************************************/
    // The final process is to remove points in between straight sections, which would cause
    // uneccessary points on our hitbox
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            // First we check for vertical lines
            /*
            Another horrible if statement, I know...
            
            The first line checks that the pixel itself is included. This is first such that we don't needlessly
            evaluate the rest of the statements when it doesn't really matter either way because the pixel is
            already not included.
            The second line checks that the pixel to the left is either not included or in the previous line
            The third line checks that the pixel to the right is either not included or in the next line
            The fourth line looks at the two pixels to the up right and up left that they are not included
            The fifth line looks at the two pixels to the down right and down left that they are not included
                
            
            if ((hitboxInclude[(i-1)*textureSize.x + j] == 1 && (i-1)*textureSize.x + j >= 0)
            && (hitboxInclude[(i+1)*textureSize.x + j] == 1 && (i+1)*textureSize.x + j + 1 < hitboxInclude.size())
            && (hitboxInclude[i*textureSize.x + j + 1] != 1)
            && (hitboxInclude[i*textureSize.x + j - 1] != 1))
            m_newHitbox[i*textureSize.x + j] = 3;
                */
            if (hitboxInclude[i*textureSize.x + j] == 1
            && ((hitboxInclude[i*textureSize.x + j - 1] == 0 || hitboxInclude[i*textureSize.x + j - 1] == 2) || i*textureSize.x + j - 1 < i*textureSize.x)
            && ((hitboxInclude[i*textureSize.x + j + 1] == 0 || hitboxInclude[i*textureSize.x + j + 1] == 2) || i*textureSize.x + j + 1 > (i+1)*textureSize.x)
            && (hitboxInclude[(i-1)*textureSize.x + j - 1] == 0 || hitboxInclude[(i-1)*textureSize.x + j - 1] == 2)
            && (hitboxInclude[(i-1)*textureSize.x + j + 1] == 0 || hitboxInclude[(i-1)*textureSize.x + j + 1] == 2)
            && (hitboxInclude[(i+1)*textureSize.x + j - 1] == 0 || hitboxInclude[(i+1)*textureSize.x + j - 1] == 2)
            && (hitboxInclude[(i+1)*textureSize.x + j + 1] == 0 || hitboxInclude[(i+1)*textureSize.x + j + 1] == 2))
            newHitbox[i*textureSize.x + j] = 3;
                
            /*
            This statement is much simpler, and the previous could probably be too, but I don't feel like doing that
            right now. This just checks that both the left and right pixels are either on a different line or are
            included.
            */
            if ((hitboxInclude[i*textureSize.x + j - 1] == 1 && i*textureSize.x + j - 1 >= i*textureSize.x)
            && (hitboxInclude[i*textureSize.x + j + 1] == 1 && i*textureSize.x + j + 1 < (i+1)*textureSize.x))
            newHitbox[i*textureSize.x + j] = 3;

        }
    }
    hitboxInclude = newHitbox;


    /*****************************************
     *      Add verticies in order
     * ******************************************

    Crunch time!
    We now have every pixel that will represent a vertex in our hitbox polygon, the problem is
    now to put them in order so that it makes a corherent shape.

    We can't just go across and add each one, because then the shape would be zig-zagged and not
    work.

    We need to find a way to assign each pixel in the outline by following the path around our
    shape.

    Since we have an outline of the sprite (where the lines should go) indicated by a 3 in our vector,
    we should be able to begin at a given pixel and move along in one direction
     */
    Vector2f currPixel;
    int vertexIndex = 0;
    vector<Vector2f> hitboxVerticies;
    hitboxVerticies.resize(hitboxInclude.size()); // This is a little overkill, but that's okay

    // Setup our polygon

    m_numVerticies = 0;
    for (int i: hitboxInclude) {
        if (i == 1)
            m_numVerticies++;
    }
    m_points.resize(m_numVerticies);

    while (vertexIndex < m_numVerticies) {
        if (hitboxInclude[currPixel.y*textureSize.x + currPixel.x] == 1 || hitboxInclude[currPixel.y*textureSize.x + currPixel.x] == 3) {
            // Even if it isn't an actual vertex, we record it in our other vector
            hitboxVerticies.push_back(currPixel);
            
            if (hitboxInclude[currPixel.y*textureSize.x + currPixel.x] == 1)
                // We record the vertex in our polygon
                m_points[vertexIndex++] = currPixel;
            // We now look for the next pixel that is marked either as a 3 or a 1
            /*
            We check starting at the top and move clockwise 
             */
            //cout << currPixel.x << " " << currPixel.y << " " << vertexIndex << endl;
            //for (Vector2f v: m_hitboxVertices)
              //  cout << v.x << " " << v.y << " --- ";

            // Top
            if ((hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x] == 1 
            || hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x] == 3)
            && ((currPixel.y - 1)*textureSize.x + currPixel.x >= 0 && (currPixel.y - 1)*textureSize.x + currPixel.x < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x, currPixel.y - 1))) {
                currPixel.x += 0;
                currPixel.y += -1;
                continue;
            } else
            // Top right
            if ((hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x + 1] == 1 
            || hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x + 1] == 3)
            && ((currPixel.y - 1)*textureSize.x + currPixel.x + 1 >= 0 && (currPixel.y - 1)*textureSize.x + currPixel.x + 1 < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x + 1, currPixel.y - 1))) {
                currPixel.x += 1;
                currPixel.y += -1;
                continue;
            } else
            // Right
            if ((hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x + 1] == 1 
            || hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x + 1] == 3)
            && ((currPixel.y)*textureSize.x + currPixel.x + 1 >= 0 && (currPixel.y)*textureSize.x + currPixel.x + 1 < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x + 1, currPixel.y))) {
                currPixel.x += 1;
                currPixel.y += 0;
                continue;
            } else
            // Bottom right
            if ((hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x + 1] == 1 
            || hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x + 1] == 3)
            && ((currPixel.y + 1)*textureSize.x + currPixel.x + 1 >= 0 && (currPixel.y + 1)*textureSize.x + currPixel.x + 1 < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x + 1, currPixel.y + 1))) {
                currPixel.x += 1;
                currPixel.y += 1;
                continue;
            } else
            // Bottom
            if ((hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x] == 1 
            || hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x] == 3)
            && ((currPixel.y + 1)*textureSize.x + currPixel.x >= 0 && (currPixel.y + 1)*textureSize.x + currPixel.x < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x, currPixel.y + 1))) {
                currPixel.x += 0;
                currPixel.y += 1;
                continue;
            } else
            // Bottom left
            if ((hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x - 1] == 1 
            || hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x - 1] == 3)
            && ((currPixel.y + 1)*textureSize.x + currPixel.x - 1 >= 0 && (currPixel.y + 1)*textureSize.x + currPixel.x - 1 < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x - 1, currPixel.y + 1))) {
                currPixel.x += -1;
                currPixel.y += 1;
                continue;
            } else
            // Left
            if ((hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x - 1] == 1 
            || hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x - 1] == 3)
            && ((currPixel.y)*textureSize.x + currPixel.x - 1 >= 0 && (currPixel.y)*textureSize.x + currPixel.x - 1 < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x - 1, currPixel.y))) {
                currPixel.x += -1;
                currPixel.y += 0;
                continue;
            } else
            // Top left
            if ((hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x - 1] == 1 
            || hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x - 1] == 3)
            && ((currPixel.y - 1)*textureSize.x + currPixel.x - 1 >= 0 && (currPixel.y - 1)*textureSize.x + currPixel.x - 1 < hitboxInclude.size())
            && !hitboxContainsPoint(hitboxVerticies, Vector2f(currPixel.x - 1, currPixel.y - 1))) {
                currPixel.x += -1;
                currPixel.y += -1;
                continue;
            } else
                break;
            
        } else {
            // Just move horizontally until we get on the path of the first vertex
            currPixel.x += 1;
        }
    }

    /*****************************************
     * 
     * END OF SHAPE GENERATION
     * 
     * It's over. It's done.
     * 
     * I sincerely apologize for that mess
     * 
     * ******************************************/

    findCentroid();
    createTriangles();
    update(); // This makes the shape actually drawable
}

/*
    The following methods are used in the above constructor
*/

bool Polygon::contains(vector<Color> vec, Color c) {
    for (Color col: vec) {
        if (col == c)
            return true;
    }
    return false;
}

bool Polygon::hitboxContainsPoint(vector<Vector2f>& hitboxVerticies, Vector2f point) {
    //cout << point.x << " " << point.y << endl;
    for (int i = 0; i < hitboxVerticies.size(); i++) {
        if (point.x == hitboxVerticies[i].x && point.y == hitboxVerticies[i].y) {
            //cout << "True" << endl;
            return true;
        }
    }
    //cout << "False" << endl;
    return false;
}


/*
    The more basic constructors

This constructor is rather basic, and I can only think of one real scenario where it would be useful:

Since most other SFML shapes have a getPoints() method, we could easily convert from some other
shape to our class here
*/

Polygon::Polygon(vector<Vector2f> points) {
    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createTriangles();
    update(); // This makes the shape actually drawable
}

Polygon::Polygon(CircleShape shape) {
    vector<Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createTriangles();
    update(); // This makes the shape actually drawable
}

Polygon::Polygon(RectangleShape shape) {
    vector<Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createTriangles();
    update(); // This makes the shape actually drawable
}

Polygon::Polygon(ConvexShape shape) {
    vector<Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createTriangles();
    update(); // This makes the shape actually drawable
}
/*
    Spitting our polygon into triangles

We always take our centroid as one of the verticies, and then te other two will just be two
consectutive points on the polygon
*/
void Polygon::createTriangles() {

    m_triangles.resize(m_numVerticies);

    for (int i = 0; i < m_points.size() - 1; i++) {
        m_triangles[i] = Triangle(m_centroid, m_points[i], m_points[i+1]);
    }

    m_triangles[m_numVerticies - 1] = Triangle(m_centroid, m_points[m_numVerticies - 1], m_points[0]);
}

/*
    Finding the centroid of our shape

The basic approach here is to create an imaginary box around our shape by finding the leftmost, rightmost
highest and lowest points, and then using the difference between these to define the dimensions. The center
point of this rectangle will then be taken as the centroid of our polygon. This may not be the exact centroid,
but it is a close enough approximation and should be fairly efficient with something like O(n) complexity,
since we only have to go through each point once to check all four extreme conditions.
*/

void Polygon::findCentroid() {

    // We are going to iterate through every vertex and find the farthest right, left, up, down points
    float left = 0.f;
    float right = 0.f;
    float top = 0.f;
    float bottom = 0.f;

    for (Vector2f v: m_points) {
        // Check each condition and reassign if its true
        if (v.x < left)
            left = v.x;
        if (v.x > right)
            right = v.x;
        if (v.y < top)
            top = v.y;
        if (v.y > bottom)
            bottom = v.y;
    }

    // Now we just take the center of our rectangle
    m_centroid.x = left + (right - left) / 2;
    m_centroid.y = top + (bottom - top) / 2;
}

size_t Polygon::getPointCount() const {
    return m_numVerticies;
}

Vector2f Polygon::getPoint(size_t index) const {
    return m_points[index];
}

vector<Vector2f> Polygon::getPoints() {
    return m_points;
}