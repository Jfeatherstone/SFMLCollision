#include "Polygon.hpp"

const float Polygon::DEFAULT_DENSITY = 1.0f;
const float Polygon::VELOCITY_THRESHOLD = .10f;

/*
The constructor that will parse our shape from a texture

We have two optional parameters here:

Detail: This could can be either Less or More (enum is defined above, see there for info) which will define either
approximate accuracy (less) or pixel perfect accuracy (more)

vector<Color>: In case we want to ignore certain parts of a sprite, we can provide their rgb values
and the parser will pass over them
*/
Polygon::Polygon(sf::Texture* texture, Detail detail, std::vector<sf::Color> ignoredColors) {
    // First we store the texture such that the polygon looks like the image
    setTexture(texture);

    ////////////////////////////////////////
    //      BEGINNING OF SHAPE GENERATION
    ////////////////////////////////////////

    /* 
    Beware!
    The next bit of code is a complete clown fiesta and I recomend scrolling past as fast as you can
    
    This code has been pasted from what used to be several methods but is now just here to avoid
    the creation of too many member variables.
    
    Another consequence of this is that the old class was solely for hitboxes (which this still kinda
    is, but not explicitly) so a lot of variables are going to be called hitboxSomething, just a heads up :/
    
    Consider yourself warned...
     
    Although it may look terrible, it actually isn't *that* bad
    Most of the code that looks horrible is actually just if statements that are very similar to each other
    The reason we need so many is because we have to slightly change the indexing of what we are checking
    to check above, below, left, right, diagonals, etc.
    
    */

    ////////////////////////////////////////
    //      Parsing RGB values from the image
    ////////////////////////////////////////
    std::vector<sf::Color> pixels;

    // Convert our texture to an image
    sf::Image image = texture->copyToImage();

    // This will be the number of pixels in the image
    int length = image.getSize().x * image.getSize().y;

    // This will happen if our image is empty (the file doesn't exist)
    if (length == 0) {
        std::cout << "Attempt to pass in empty image to constructor!\n In Polygon::Polygon(sf::Texture* texture, Detail detail, std::vector<sf::Color> ignoredsf::Colors)" << std::endl;
        return;
    }

    // Reassign our pixel std::vector
    pixels.clear();
    pixels.resize(length);

    // The actual array of pixels
    const sf::Uint8* arr = image.getPixelsPtr();


    // Take the average color to find a background color
    sf::Vector3f backgroundAverage;

    // Separate out each color and create an object to add to our std::vector
    for (int i = 0; i < length; i++) {
        int red = (int) arr[4*i];
        int green = (int) arr[4*i + 1];
        int blue = (int) arr[4*i + 2];
        int alpha = (int) arr[4*i + 3];
        sf::Color c(red, green, blue, alpha);

        //std::cout << float(red) << " " << float(blue) << " " << float(green) << " " << float(alpha) << "||";
        backgroundAverage += sf::Vector3f(float(red), float(green), float(blue));

        pixels[i] = c;
    }

    float backgroundTolerance = 7.f;

    backgroundAverage.x /= length;
    backgroundAverage.y /= length;
    backgroundAverage.z /= length;

    std::cout << pixels[0].r - backgroundAverage.x << " " << pixels[0].g - backgroundAverage.y << " " << pixels[0].b - backgroundAverage.z << std::endl;

    std::cout << (pixels[0].r - backgroundAverage.x <= backgroundTolerance
            && pixels[0].g - backgroundAverage.y <= backgroundTolerance
            && pixels[0].b - backgroundAverage.z <= backgroundTolerance) << std::endl;
    ///////////////////////////////////////////
    //     Create our first set of vertices
    ///////////////////////////////////////////
    sf::Vector2i textureSize;

    textureSize.x = texture->getSize().x;
    textureSize.y = texture->getSize().y;

    std::cout << textureSize.x << " " << textureSize.y << std::endl;

    int preCropPixels[textureSize.y][textureSize.x];

    std::cout << "Pre crop\n";

    for (int i = 0; i < length; i++) {
        if (!(pixels[i].r - backgroundAverage.x <= backgroundTolerance
            && pixels[i].g - backgroundAverage.y <= backgroundTolerance
            && pixels[i].b - backgroundAverage.z <= backgroundTolerance))
                pixels[i] = sf::Color(0, 0, 0, 0);
            else
                std::cout << "Not removed" << std::endl;
    }

    // Next, we want to go through every color and if it isn't empty or on the ignore list, we
    // add it to our include for the hitbox calculation

    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {

            // Hold onto the color to clean up the code
            sf::Color c = pixels[i*textureSize.x + j];

            //std::cout << i << " " << j << " - " << int(c.r) << " " << int(c.g) << " " << int(c.b) << " " << int(c.a) << std::endl;

            // If the color is not ignored and has a non-zero alpha value, we keep it
            if (!contains(ignoredColors, c) && c.a > 0)
                preCropPixels[i][j] = 1;
            else
                preCropPixels[i][j] = 0;
        }
    }

    std::cout << "Post crop\n";
    
    // Now, we want to crop the image properly to remove excess space
    // We are going to iterate through every vertex and find the farthest right, left, up, down points
    int left = textureSize.x;
    int right = 0;
    int top = textureSize.y;
    int bottom = 0;
    
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {

            //std::cout << hitboxInclude[i*textureSize.x + j] << std::endl;

            if (preCropPixels[i][j] == 1) {

                // Check each condition and reassign if its true
                if (j < left)
                    left = j;
                if (j > right)
                    right = j;
                if (i < top)
                    top = i;
                if (i > bottom)
                    bottom = i;
            }
        }
    }

    //std::cout << left << " " << right << " " << top << " " << bottom << std::endl;

    // There are four regions that need to be erased:
    // above, below, left, and right

    int newArr[bottom - top + 1][right - left + 1];
    
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {

            if (i < top || i > bottom || j < left || j > right) {
                //std::cout << "-";
                continue;
            }

            newArr[i - top][j - left] = (preCropPixels[i][j]);
            //std::cout << newArr[i - top][j - left];
        }

    //std::cout << std::endl;
    }

    // The +1 makes it so that we don't lose the last right and bottom rows
    // Not sure why they get cut off otherwise, but they do
    /*int** includedPixels;
    includedPixels = new int*[bottom - top + 1];
    for (int i = 0; i < bottom - top + 1; i++)
        includedPixels[i] = new int[right - left + 1];
    */
    int includedPixels[bottom - top + 1][right - left + 1];

    std::cout << "After arr" << std::endl;

    // And copy the values over (I was having some trouble using std::copy)
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            includedPixels[i][j] = newArr[i][j];
        }
    }

    std::cout << "After assignment" << std::endl;

    textureSize.x = right - left + 1;
    textureSize.y = bottom - top + 1;
    
    // Now we crop the texture to remove excess space (pixels)

    ///*
    ///////////////////////////////////////////////
    // Print out our current vertices to help debug
    std::cout << std::endl;
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            std::cout << includedPixels[i][j];
        }
    std::cout << std::endl;
    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/


    ///////////////////////////////////////////
    //     Fill in the inside
    ///////////////////////////////////////////
    /*
    We want to check, for every point that has a value of 0, whether it is bounded on all four sides as well as on the diagonals
        */
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            if (includedPixels[i][j] == 0) {
                bool right = false, left = false, up = false, down = false; // Our cardinal directions
                bool upright = false, downright = false, upleft = false, downleft = false; // We also want to check diagonals
                // Right
                for (int k = j+1; k < textureSize.x; k++) {
                    if (includedPixels[i][k] == 1) {
                        right = true;
                        break;
                    }
                }
                // Left
                for (int k = j-1; k >= 0; k--) {
                    if (includedPixels[i][k] == 1) {
                        left = true;
                        break;
                    }
                }
                // Up
                for (int k = i-1; k >= 0; k--) {
                    if (includedPixels[k][j] == 1) {
                        up = true;
                        break;
                    }
                }
                // Down
                for (int k = i+1; k < textureSize.y; k++) {
                    if (includedPixels[k][j] == 1) {
                        down = true;
                        break;
                    }
                }
                // Up Right
                /*
                The sum in the conditional part of this loop is not a precise amount, since the diagonals
                could be a few blocks long or much longer, but this value will assure that we will never
                check too little values, but it could probably be optimized later
                */
                for (int k = 0; k < textureSize.x + textureSize.y; k++) {
                    // We also will have another if as to optimize our code
                    if (i-k < 0 || j + k > textureSize.x)
                        break;
                    
                    if (includedPixels[i - k][j + k] == 1) {
                        upright = true;
                        break;
                    }
                }
                // Down right
                for (int k = 0; k < textureSize.x + textureSize.y; k++) {
                    // We also will have another if as to optimize our code
                    if (i+k > textureSize.y || j + k > textureSize.x)
                        break;
                    
                    if (includedPixels[i + k][j + k] == 1) {
                        downright = true;
                        break;
                    }
                }
                // Down left
                for (int k = 0; k < textureSize.x + textureSize.y; k++) {
                    // We also will have another if as to optimize our code
                    if (i+k > textureSize.y || j - k < 0)
                        break;
                    
                    if (includedPixels[i + k][j - k] == 1) {
                        downleft = true;
                        break;
                    }
                }
                // Up left
                for (int k = 0; k < textureSize.x + textureSize.y; k++) {
                    // We also will have another if as to optimize our code
                    if (i-k < 0 || j - k < 0)
                        break;
                    
                    if (includedPixels[i - k][j - k] == 1) {
                        upleft = true;
                        break;
                    }
                }

                if (left && right && up && down && upleft && upright && downleft && downright) {
                    includedPixels[i][j] = 1;
                }
            }
        }
    } 

    ///*
    ///////////////////////////////////////////////
    // Print out our current vertices to help debug
    std::cout << std::endl;
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            std::cout << includedPixels[i][j];
        }
    std::cout << std::endl;
    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    ///////////////////////////////////////////
    //     Removing the inside
    ///////////////////////////////////////////
    // Now, we go through and remove everything except for the outline
    int newPixels[textureSize.y][textureSize.x];

    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            // We check if everything surrounding it is a 1 which would denote something inside
            /*
            An explanation of this ungodly if statement:
            To determine whether a pixel is inside of the sprite -- and therefore not needed to determine the
            hitbox -- we have to look at any of the pixels around it.
            The first line of the statement makes sure that the value to the left is included in the hitbox, and
            that it isn't on the end of the previous line (since this isn't actually a 2d array, rather its 1d)

            The second line does the same as the first, but with the right side, checking that the value is 1 and
            that it isn't past the end of the line (onto the next one)

            Next, the line checks that both above and below the pixel are included, which don't need weird
            cases like left and right since they are just up and down

            On the fourth line, we make sure that we aren't at the bottom line. This was causing an error where some
            on the last line weren't included and I have no idea why, but this seemed like a good way
            to fix it :)

            The last two lines look at the diagonals
            */
            newPixels[i][j] = includedPixels[i][j];

            if ((includedPixels[i][j - 1] == 1 && i*textureSize.x + j - 1 >= i*textureSize.x)
            && (includedPixels[i][j + 1] == 1 && i*textureSize.x + j + 1 < (i+1)*textureSize.x)
            && (includedPixels[i - 1][j] == 1) && (includedPixels[i + 1][j] == 1)
            && (i + 1 < textureSize.y && i - 1 >= 0)
            && (includedPixels[i - 1][j - 1] == 1) && (includedPixels[i - 1][j + 1] == 1)
            && (includedPixels[i + 1][j - 1] == 1) && (includedPixels[i + 1][j + 1] == 1))            
                newPixels[i][j] = 2;
        }
    }

    // And copy the values over (I was having some trouble using std::copy)
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            includedPixels[i][j] = newPixels[i][j];
        }
    }

    ///*
    ///////////////////////////////////////////////
    // Print out our current vertices to help debug
    std::cout << std::endl;
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            std::cout << includedPixels[i][j];
        }
    std::cout << std::endl;
    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/


    /*****************************************
     *      Remove excess vertices
     * 
     * 311 -> 331
     * 
     * 111 -> 131
     * 
     * 113 -> 133
     * 
     * 1    1
     * 1 -> 3
     * 1    1
     * 
     * ******************************************/
    // The final process is to remove points in between straight sections, which would cause
    // uneccessary points on our hitbox

    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            
            // We don't change inside points
            if (includedPixels[i][j] == 2)
                continue;

            // This just checks that both the left and right pixels are either on a different line or are included.
            if (((includedPixels[i][j - 1] == 1 || includedPixels[i][j - 1] == 3) && j - 1 >= 0)
            && ((includedPixels[i][j + 1] == 1 || includedPixels[i][j + 1] == 3) && j + 1 < textureSize.x))
                includedPixels[i][j] = 3;
            
            // Now we do the same, but vertically
            if (((includedPixels[i - 1][j] == 1 || includedPixels[i - 1][j] == 3) && i - 1 >= 0)
            && ((includedPixels[i + 1][j] == 1 || includedPixels[i + 1][j] == 3) && i + 1 < textureSize.y))
                includedPixels[i][j] = 3;

        }
    }

    ///*
    ///////////////////////////////////////////////
    // Print out our current vertices to help debug
    std::cout << std::endl;
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            std::cout << includedPixels[i][j];
        }
    std::cout << std::endl;
    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/


    /*
    Remove diagonal vertices

    What this method  does:

    001     001
    011 ->  010
    110     100

    It removes the "squareness" of diagonal lines by removing extra vertices
    */
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {

            // Top right diagonal
            if (includedPixels[i][j] == 1 // The current point is a vertex
            && includedPixels[i - 1][j + 1] == 1 // The top right point is a vertex
            && i - 1 >= 0 && j + 1 < textureSize.x) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (includedPixels[i][j + 1] == 1)
                    includedPixels[i][j + 1] = 0; // right
                if (includedPixels[i - 1][j] == 1) 
                    includedPixels[i - 1][j] = 0; // above
            }

            // Bottom right diagonal
            if (includedPixels[i][j] == 1 // The current point is a vertex
            && includedPixels[i + 1][j + 1] == 1 // The bottom right point is a vertex
            && i + 1 < textureSize.y && j + 1 < textureSize.x) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (includedPixels[i][j + 1] == 1)
                    includedPixels[i][j + 1] = 0; // right
                if (includedPixels[i + 1][j] == 1) 
                    includedPixels[i + 1][j] = 0; // below
            }

            // Bottom left diagonal
            if (includedPixels[i][j] == 1 // The current point is a vertex
            && includedPixels[i + 1][j - 1] == 1 // The bottom left point is a vertex
            && i + 1 < textureSize.y  && j - 1 >= 0) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (includedPixels[i][j - 1] == 1)
                    includedPixels[i][j - 1] = 0; // left
                if (includedPixels[i + 1][j] == 1) 
                    includedPixels[i + 1][j] = 0; // below
            }

            // Top left diagonal
            if (includedPixels[i][j] == 1 // The current point is a vertex
            && includedPixels[i - 1][j - 1] == 1 // The top right point is a vertex
            && i - 1 >= 0 && j + 1 < textureSize.x) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (includedPixels[i][j - 1] == 1)
                    includedPixels[i][j - 1] = 0; // left
                if (includedPixels[i - 1][j] == 1) 
                    includedPixels[i - 1][j] = 0; // above
            }

        }
    }

    ///*
    ///////////////////////////////////////////////
    // Print out our current vertices to help debug
    std::cout << std::endl;
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            std::cout << includedPixels[i][j];
        }
    std::cout << std::endl;
    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    /*
    Remove diagonal vertices part 2 - Lines

    What this method  does:

    001     001
    010 ->  030
    100     100

    */
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            /*
            What we are doing here is the same as we did for straight lines before
            We want to remove any intermediate points from the line and only leave the endpoints
            */
            if (includedPixels[i][j] == 1
            && (includedPixels[i - 1][j - 1] == 1 || includedPixels[i - 1][j - 1] == 3)
            && (includedPixels[i + 1][j + 1] == 1 || includedPixels[i + 1][j + 1] == 3)
            && includedPixels[i][j + 1] != 1 && includedPixels[i][j - 1] != 1
            && includedPixels[i + 1][j] != 1 && includedPixels[i - 1][j] != 1)
                includedPixels[i][j] = 3;

            if (includedPixels[i][j] == 1
            && (includedPixels[i + 1][j - 1] == 1 || includedPixels[i + 1][j - 1] == 3)
            && (includedPixels[i - 1][j + 1] == 1 || includedPixels[i - 1][j + 1] == 3)
            && includedPixels[i][j + 1] != 1 && includedPixels[i][j - 1] != 1
            && includedPixels[i + 1][j] != 1 && includedPixels[i - 1][j] != 1)
                includedPixels[i][j] = 3;

        }
    }

    ///*
    ///////////////////////////////////////////////
    // Print out our current vertices to help debug
    std::cout << std::endl;
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            std::cout << includedPixels[i][j];
        }
    std::cout << std::endl;
    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    //////////////////////////////////////////
    //      Add vertices in order
    //////////////////////////////////////////
    /*
    Crunch time!
    We now have every pixel that will represent a vertex in our hitbox polygon, the problem is
    now to put them in order so that it makes a corherent shape.

    We can't just go across and add each one, because then the shape would be zig-zagged and not
    work.

    We need to find a way to assign each pixel in the outline by following the path around our
    shape.

    Since we have an outline of the sprite (where the lines should go) indicated by a 3 in our array,
    we should be able to begin at a given pixel and move along in one direction
    */
    sf::Vector2i currPixel;
    sf::Vector2f centerPoint(textureSize.x / 2.0f, textureSize.y / 2.0f);

    int vertexIndex = 0;
    std::vector<sf::Vector2f> polygonvertices;
    //hitboxvertices.resize(hitboxInclude.size()); // This is a little overkill, but that's okay

    // Setup our polygon

    // We need to know how many vertices we have
    m_numVertices = 0;
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            if (includedPixels[i][j] == 1)
                m_numVertices++;

        }
    }
    
    m_points.resize(m_numVertices);


    sf::Vector2f directions[8] = {
        sf::Vector2f(0, -1), // Top
        sf::Vector2f(1, 0), // Right
        sf::Vector2f(0, 1), // Bottom
        sf::Vector2f(-1, 0), // Left
        sf::Vector2f(1, -1), // Top right
        sf::Vector2f(1, 1), // Bottom right
        sf::Vector2f(-1, 1), // Bottom left
        sf::Vector2f(-1, -1), // Top left
    };


    std::cout << "numVericies before adding: " << m_numVertices << std::endl;

    while (vertexIndex < m_numVertices) {
        if (includedPixels[currPixel.y][currPixel.x] == 1 || includedPixels[currPixel.y][currPixel.x] == 3) {
            // Even if it isn't an actual vertex, we record it in our other std::vector
            polygonvertices.push_back(sf::Vector2f(currPixel.x, currPixel.y));
            //std::cout << currPixel.x << " " << currPixel.y;

            if (includedPixels[currPixel.y][currPixel.x] == 1) {
                // We record the vertex in our polygon
                //std::cout << " - Added " << vertexIndex << std::endl;
                m_points[vertexIndex++] = sf::Vector2f(currPixel.x, currPixel.y);
            } else {
                //std::cout << std::endl;
            }

            // We now look for the next pixel that is marked either as a 3 or a 1
            // We check starting at the top and move clockwise 
            
            //cout << currPixel.x << " " << currPixel.y << " " << vertexIndex << endl;
            //for (sf::Vector2f v: m_hitboxVertices)
            //    cout << v.x << " " << v.y << " --- ";

            bool foundPoint = false;

            // Iterate over every direction (in clockwise direction)
            for (sf::Vector2f vec: directions) {

                //std::cout << vec.x << " " << vec.y << std::endl;

                if ((includedPixels[int(currPixel.y + vec.y)][int(currPixel.x + vec.x)] == 1 // The new pixel is either a 1
                ||   includedPixels[int(currPixel.y + vec.y)][int(currPixel.x + vec.x)] == 3) // or a 3
                && (currPixel.x + vec.x >= 0 && currPixel.x + vec.x < textureSize.x) // The x coordinate is within [0, width)
                && (currPixel.y + vec.y >= 0 && currPixel.y + vec.y < textureSize.y) // The y coordinate is within [0, height)
                && (!contains(polygonvertices, sf::Vector2f(currPixel.x + vec.x, currPixel.y + vec.y)))) { // And the pixel hasn't already been added

                    currPixel.x += vec.x;
                    currPixel.y += vec.y;
                    foundPoint = true;
                    break;
                }
            }

            if (!foundPoint)
                break;

            /*
            // Top
            if ((hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x] == 1 
            || hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x] == 3)
            && ((currPixel.y - 1) >= 0 && (currPixel.y - 1)*textureSize.x + currPixel.x < hitboxInclude.size())
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x, currPixel.y - 1))) {
                currPixel.x += 0;
                currPixel.y += -1;
                previousMovement = sf::Vector2f(0, -1);
                continue;
            } else
            // Top right
            if ((hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x + 1] == 1 
            || hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x + 1] == 3)
            && ((currPixel.y - 1) >= 0 && (currPixel.y - 1)*textureSize.x + currPixel.x + 1 < hitboxInclude.size())
            && (currPixel.x + 1 < textureSize.x)
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x + 1, currPixel.y - 1))) {
                currPixel.x += 1;
                currPixel.y += -1;
                previousMovement = sf::Vector2f(1, -1);
                continue;
            } else
            // Right
            if ((hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x + 1] == 1 
            || hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x + 1] == 3)
            && ((currPixel.y)*textureSize.x + currPixel.x + 1 >= 0 && (currPixel.y)*textureSize.x + currPixel.x + 1 < hitboxInclude.size())
            && (currPixel.x + 1 < textureSize.x)
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x + 1, currPixel.y))) {
                currPixel.x += 1;
                currPixel.y += 0;
                previousMovement = sf::Vector2f(1, 0);
                continue;
            } else
            // Bottom right

            if ((hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x + 1] == 1 
            || hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x + 1] == 3)
            && ((currPixel.y + 1)*textureSize.x + currPixel.x + 1 >= 0 && (currPixel.y + 1)*textureSize.x + currPixel.x + 1 < hitboxInclude.size())
            && (currPixel.x + 1 < textureSize.x)
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x + 1, currPixel.y + 1))) {
                currPixel.x += 1;
                currPixel.y += 1;
                previousMovement = sf::Vector2f(1, 1);
                continue;
            } else
            // Bottom
            if ((hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x] == 1 
            || hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x] == 3)
            && ((currPixel.y + 1)*textureSize.x + currPixel.x >= 0 && (currPixel.y + 1)*textureSize.x + currPixel.x < hitboxInclude.size())
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x, currPixel.y + 1))) {
                currPixel.x += 0;
                currPixel.y += 1;
                previousMovement = sf::Vector2f(0, 1);
                continue;
            } else
            // Bottom left
            if ((hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x - 1] == 1 
            || hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x - 1] == 3)
            && ((currPixel.y + 1)*textureSize.x + currPixel.x - 1 >= 0 && (currPixel.y + 1)*textureSize.x + currPixel.x - 1 < hitboxInclude.size())
            && (currPixel.x - 1 >= 0)
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x - 1, currPixel.y + 1))) {
                currPixel.x += -1;
                currPixel.y += 1;
                previousMovement = sf::Vector2f(-1, 1);
                continue;
            } else
            // Left
            if ((hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x - 1] == 1 
            || hitboxInclude[(currPixel.y)*textureSize.x + currPixel.x - 1] == 3)
            && ((currPixel.y)*textureSize.x + currPixel.x - 1 >= 0 && (currPixel.y)*textureSize.x + currPixel.x - 1 < hitboxInclude.size())
            && (currPixel.x - 1 >= 0)
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x - 1, currPixel.y))) {
                currPixel.x += -1;
                currPixel.y += 0;
                previousMovement = sf::Vector2f(-1, 0);
                continue;
            } else
            // Top left
            if ((hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x - 1] == 1 
            || hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x - 1] == 3)
            && ((currPixel.y - 1)*textureSize.x + currPixel.x - 1 >= 0 && (currPixel.y - 1)*textureSize.x + currPixel.x - 1 < hitboxInclude.size())
            && (currPixel.x - 1 >= 0)
            && !contains(hitboxvertices, sf::Vector2f(currPixel.x - 1, currPixel.y - 1))) {
                currPixel.x += -1;
                currPixel.y += -1;
                previousMovement = sf::Vector2f(-1, -1);
                continue;
            } else {
                //cout << "Done" << endl;
                break;
            }
            */
        } else {
            // Just move horizontally until we get on the path of the first vertex
            currPixel.x += 1;
            // And reset if we get to the end
            if (currPixel.x >= textureSize.x) {
                currPixel.x = 0;
                currPixel.y++;
            }
            
        }
    }

    std::cout << "Final vertex index: " << vertexIndex << std::endl;

    ///*
    // 0, 0 has been causing some trouble, so we remove it if it isn't actually there
    if (includedPixels[0][0] != 1) {
        //cout << "Excess zero present" << endl;
        for (int i = 0; i < m_points.size(); i++) {
            if (m_points[i] == sf::Vector2f(0, 0)) {
                m_points.erase(m_points.begin() + i);
                i--; // So we don't skip the following one
            }
        }
    }
    
    // Also remove neagtive points because it makes no sense (since our origin always starts
    // at (0, 0)
    // Later on it is fine to have negative points, but not rn
    for (int i = 0; i < m_points.size(); i++) {
        if (m_points[i].x < 0 || m_points[i].y < 0) {
            m_points.erase(m_points.begin() + i);
            i--;
        }
    }
    //*/

    // Update the size of our vertices
    m_numVertices = m_points.size();

    //////////////////////////////////////////
    // 
    // END OF SHAPE GENERATION
    // 
    // It's over. It's done.
    //
    // I sincerely apologize for that mess
    //////////////////////////////////////////
    
    //for (sf::Vector2f v: m_points) {
    //    std::cout << v.x << " " << v.y << std::endl;
    //}

    /*
    Area optimization
    TODO: Make this better
    */
    // We only run this if we are trying to optimize vertices (i.e. not Exact detail)
    if (detail != Detail::Exact) {
        // Depending on our level of detail, we have a higher or lower tolerance
        // for the change in the area when removing points
        float diff = .03f;
        if (detail == Detail::Less)
            diff = .1f;
        if (detail == Detail::More)
            diff = .01f;

        while (true) {
            // We want to be able to keep track of whether or not we were actually able to reduce the shape
            bool reduced = false;
            for (int i = 0; i < m_points.size(); i++) {
                // Store the old points to calculate the previous area
                std::vector<sf::Vector2f> old = m_points;
                
                // Yeet that point out of here
                m_points.erase(m_points.begin() + i);

                // Calculate our two areas
                float a1, a2;
                Polygon::calculateArea(m_points, a1);
                Polygon::calculateArea(old, a2);

                // Now we check if they are within the tolerance governed by our level of detail
                float dA = abs(a2 - a1);
                if (dA / a2 >= diff)
                    // If the difference is too much, revert the change
                    m_points = old;
                else
                    // Otherwise we keep it
                    reduced = true;
            }
            // If we weren't able to do anything, we stop trying to reduce
            if (!reduced)
                break;
        }
    }

    m_numVertices = m_points.size();

    // Copy over our array into the member variable
    m_includedPixels.resize(textureSize.y);
    for (int i = 0; i < textureSize.y; i++) {
        m_includedPixels[i].resize(textureSize.x);

        for (int j = 0; j < textureSize.x; j++)
            m_includedPixels[i][j] = includedPixels[i][j];
    }

    //std::cout << "Final vertices:\n";
    //for (sf::Vector2f v: m_points)
    //    std::cout << v.x << " " << v.y << std::endl;

    findCentroid();
    createLines();
    calculateMass();
    calculateMomentOfInertia();
    Shape::update(); // This makes the shape actually drawable

    m_wasGeneratedFromImage = true;

}

/////////////////////////////////////////////////////////////
//    The following methods are used in the above constructor
/////////////////////////////////////////////////////////////

bool Polygon::contains(std::vector<sf::Color>& vec, sf::Color c) {
    //std::cout << "Before vec call\n";
    //std::cout << vec.size();
    //std::cout << "After vec call\n";
    /*
    We use this method in determining whether a list of colors to be ignored was provided
    As we read every pixel from an image, we check whether the ignoredsf::Colors std::vector
    contains the rgba value
    */
    for (sf::Color col: vec) {
        std::cout << "Loop";
        if (col == c)
            return true;
    }
    //std::cout << std::endl;
    return false;
}

bool Polygon::contains(std::vector<sf::Vector2f>& vec, sf::Vector2f point) {
    //cout << point.x << " " << point.y << endl;
    for (int i = 0; i < vec.size(); i++) {
        if (point.x == vec[i].x && point.y == vec[i].y) {
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

Polygon::Polygon(sf::CircleShape shape) {
    m_points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        m_points[i] = shape.getPoint(i);
    }

    m_numVertices = m_points.size();

    findCentroid();
    createLines();
    calculateMass();
    calculateMomentOfInertia();
    Shape::update(); // This makes the shape actually drawable
}

Polygon::Polygon(sf::RectangleShape shape) {
    m_points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        m_points[i] = shape.getPoint(i);
    }

    m_numVertices = m_points.size();

    findCentroid();
    createLines();
    calculateMass();
    calculateMomentOfInertia();
    Shape::update(); // This makes the shape actually drawable
}

Polygon::Polygon(sf::ConvexShape shape) {
    m_points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        m_points[i] = shape.getPoint(i);
    }

    m_numVertices = m_points.size();

    findCentroid();
    calculateMomentOfInertia();
    createLines();
    calculateMass();
    Shape::update(); // This makes the shape actually drawable
}


void Polygon::findCentroid() {
    /*
    The basic approach here is to create an imaginary box around our shape by finding the leftmost, rightmost
    highest and lowest points, and then using the difference between these to define the dimensions. The center
    point of this rectangle will then be taken as the centroid of our polygon. This may not be the exact centroid,
    but it is a close enough approximation and should be fairly efficient with something like O(n) complexity,
    since we only have to go through each point once to check all four extreme conditions.
    */ 

    // We are going to iterate through every vertex and find the farthest right, left, up, down points
    float left = 0.f;
    float right = 0.f;
    float top = 0.f;
    float bottom = 0.f;
    
    for (sf::Vector2f v: m_points) {
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

void Polygon::createLines() {
    //std::cout << "Creating Lines" << std::flush;

    /*
    This method is mostly linear algebra and (more or less) simple transformations on our std::vector of points.
    First, we rotate our points, around the origin of the shape, followed by scaling them up, and finally
    by adding the offset of the shape (its position).
    */

    /*
    We don't want any of the transformations we do to be permanant, so we calculate the new points, and then
    revert to the old so when we then rotate, translate, or scale, we don't have to do some weird math to
    account for the previous transformations of the points.
    */
    std::vector<sf::Vector2f> pointsCopy = m_points;

    /*
    The first transformation we do on our object is a rotation of the points. This is followed by the scale transform,
    for no particular reason as both transformations should be commuatative (a rotated then scaled object is the same 
    as a scaled then rotated object)
    The rotation is pretty straight forward and defined in the VectorMath class, but the scale has a bit of a caveat.
    Since we have to scale the object about its origin (since SFML does this) we need to account for the change by
    first translating such that the origin is at (0, 0), multiplying by the scale factors, and then translating
    back.
    */ 
    for (int i = 0; i < m_numVertices; i++) {
        VectorMath::rotate(m_points[i], getOrigin(), getRotation());
        
        // Align our point such that the origin becomes (0, 0)
        m_points[i].x -= getOrigin().x;
        m_points[i].y -= getOrigin().y;

        // Scale it
        m_points[i].x *= getScale().x;
        m_points[i].y *= getScale().y;

        // Translate our scaled point relative to its original origin
        m_points[i].x += getOrigin().x;
        m_points[i].y += getOrigin().y;
    }
    
    /*
    Now that our first two transformations are out of the way, we have to translate the points to their actual position
    on the screen. This offset is calculated using the origin and position.
    */ 
    sf::Vector2f offset(getPosition().x - getOrigin().x, getPosition().y - getOrigin().y);
    for (int i = 0; i < m_numVertices; i++) {
        m_points[i].x += offset.x;
        m_points[i].y += offset.y; 
    }

    /*
    Now that our points properly represent the shape we want, we can create lines between them to check for collisions
    */

    m_lines.clear();
    m_lines.resize(m_numVertices);
    
    for (int i = 0; i < m_points.size() - 1; i++) {
        m_lines[i] = Line(m_points[i], m_points[i+1]);
    }

    m_lines[m_numVertices - 1] = Line(m_points[m_numVertices - 1], m_points[0]);

    m_points = pointsCopy;   

    if (m_wasGeneratedFromImage) {
        // Now we create our normals
        // We want to iterate over every line
        for (int i = 0; i < m_lines.size(); i++) {

            // We first take a guess at the normal (we may need to take the negative of it)
            sf::Vector2f normalGuess = m_lines[i].getNormal();

            // Next we take the center of the line as our sample point
            sf::Vector2f lineCenter = (m_lines[i].getEnd() + m_lines[i].getStart()) / 2.0f;

            // Now we have to accout for the fact that the line center takes the position into account
            lineCenter -= sf::Vector2f(getPosition().x - (getOrigin().x) * getScale().x, getPosition().y - (getOrigin().y) * getScale().y);
            lineCenter.x /= getScale().x;
            lineCenter.y /= getScale().y;

            // After this, we add our normal and see if we end up in an inside square
            // We have to cast to ints so that we have values at those indicies in the included pixels arr
            // I use twice the magnitude of the guess since I want to make sure I don't end up on a boundary line
            // I found this out just by experimenting, so there may be a better way
            sf::Vector2i newPoint = sf::Vector2i(int(lineCenter.x - 2. * normalGuess.x), int(lineCenter.y - 2. * normalGuess.y));

            // Make sure it isn't out of bounds
            sf::Vector2i textureSize(m_includedPixels[0].size(), m_includedPixels.size());
            // If the normal is going out of bounds, it is probably in the correct direction
            if (newPoint.x >= textureSize.x || newPoint.x < 0 || newPoint.y >= textureSize.y || newPoint.y < 0)
                continue;

            //std::cout << newPoint.x << " " << newPoint.y << std::endl;

            //std::cout << normalGuess.x << " " << normalGuess.y << std::endl;
            //std::cout << m_includedPixels[newPoint.y][newPoint.x] << std::endl;


            // Flip it
            if (m_includedPixels[newPoint.y][newPoint.x] == 2) {
                normalGuess = -normalGuess;
                //std::cout << "Flipped " << i << std::endl;
            }

            // And set the normal
            m_lines[i].setNormal(normalGuess);
            //std::cout << m_lines[i].getNormal().x << " " << m_lines[i].getNormal().y << std::endl;

        } 

    } else {
        // If the line was not generated, that means it must be convex, so this process becomes easier
        for (int i = 0; i < m_lines.size(); i++) {
            // We first take a guess at the normal (we may need to take the negative of it)
            sf::Vector2f normalGuess = m_lines[i].getNormal();

            // Next we take the center of the line as our sample point
            sf::Vector2f lineCenter = (m_lines[i].getEnd() + m_lines[i].getStart()) / 2.0f;

            // Now we have to accout for the fact that the line center takes the position into account
            lineCenter -= sf::Vector2f(getPosition().x - (getOrigin().x) * getScale().x, getPosition().y - (getOrigin().y) * getScale().y);
            lineCenter.x /= getScale().x;
            lineCenter.y /= getScale().y;

            // After this, we add our normal and see if we end up in an inside square
            // We have to cast to ints so that we have values at those indicies in the included pixels arr
            sf::Vector2f newPoint = lineCenter - normalGuess;

            if (VectorMath::mag(getCentroid() - newPoint) < VectorMath::mag(getCentroid() - lineCenter))
                normalGuess = -normalGuess;

            m_lines[i].setNormal(normalGuess);

        }
    }

    m_lineUpdateRequired = false;
}

void Polygon::calculateMass() {
    Polygon::calculateArea(getPoints(), m_area);
    m_mass = m_density * m_area;
}

void Polygon::calculateMomentOfInertia() {
    // We assume a uniform distribution of density throughout the object
    // given by m_density

    /*
    I = \int{r^2 dm}
    Instead of using every point in the shape, we instead just use the vertices. This won't give us the exact moment of inertia
    in a physical sense, but will give a value that is proportional to it, making all relative comparisons between shapes accurate

    This makes the calculation super easy, as we just add up the distance from the origin to the points\
    */
    m_centerOfMass.x = 0;
    m_centerOfMass.y = 0;
    m_momentOfInertia = 0;

    for (sf::Vector2f p: m_points) {
        m_centerOfMass += p;
    }

    m_centerOfMass.x /= m_numVertices;
    m_centerOfMass.y /= m_numVertices;

    // Now find the average distance from the center of mass to the points
    for (sf::Vector2f p: m_points) {
        m_momentOfInertia += VectorMath::mag(m_centerOfMass - p);
    }

    m_momentOfInertia /= getPointCount();
    m_momentOfInertia *= getMass();

    //std::cout << m_momentOfInertia << std::endl;

}

///////////////////////////////////////
//        VERTEX INFO
///////////////////////////////////////

size_t Polygon::getPointCount() const {
    return m_numVertices;
}

sf::Vector2f Polygon::getPoint(size_t index) const {
    return m_points[index];
}

std::vector<sf::Vector2f> Polygon::getPoints() {
    return m_points;
}

std::vector<Line> Polygon::getLines() {
    if (m_lineUpdateRequired)
        createLines();

    return m_lines;
}

sf::Vector2f Polygon::getCentroid() {
    return m_centroid;
}


///////////////////////////////////////
//        PHYSICAL PROPERTIES
///////////////////////////////////////

void Polygon::setSolid(bool state) {
    m_isSolid = state;
}

bool Polygon::isSolid() {
    return m_isSolid;
}

void Polygon::setDegreesOfFreedom(bool canBeMovedLinearly, bool canBeRotated) {
    m_linearFreedom = canBeMovedLinearly;
    m_rotationalFreedom = canBeRotated;
}

bool Polygon::getLinearFreedom() {
    return m_linearFreedom;
}

bool Polygon::getRotationalFreedom() {
    return m_rotationalFreedom;
}

void Polygon::setDensity(float newDensity) {
    m_density = newDensity;
    // Now recalculate the mass
    calculateMass();
    calculateMomentOfInertia();
}

float Polygon::getDensity() {
    return m_density;
}

float Polygon::getMass() {
    return m_mass;
}

float Polygon::getMomentOfInertia() {
    return m_momentOfInertia;
}

sf::Vector2f Polygon::getCenterOfMass() {
    return m_centerOfMass;
}

float Polygon::getYoungsModulus() {
    return m_youngsModulus;
}

void Polygon::setYoungsModulus(float youngsModulus) {
    m_youngsModulus = youngsModulus;
}

float Polygon::getGamma() {
    return m_gamma;
}

void Polygon::setGamma(float gamma) {
    m_gamma = gamma;
}

void Polygon::calculateArea(std::vector<sf::Vector2f> points, float& value) {
    value = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        float avgY = (points[i].y + points[i+1].y) / 2;
        float dX = (points[i+1].x - points[i].x);
        value += avgY*dX;
    }
    value += ((points[points.size()-1].y + points[0].x) / 2) * (points[0].x - points[points.size()-1].y);
    value *= -1;
    //cout << value << endl;
}

float Polygon::getArea() {
     return m_area;
}

///////////////////////////////////////
//              MOTION
///////////////////////////////////////


void Polygon::addForce(Force force) {
    m_forces.push_back(force);
}

/**
 * @brief Updates the shape and applies both linear and angular velocity to update the
 * position and rotation of the polygon
 * 
 * @param elapsedTime The amount of time that has elapsed since the last update
 */
void Polygon::update(float elapsedTime) {

    //if (VectorMath::mag(m_velocity) <= VELOCITY_THRESHOLD)
    //    m_velocity = sf::Vector2f(0, 0);

    // Apply any forces on the object
    applyForces();

    // Update the position
    setPosition(getPosition() + m_velocity * elapsedTime);
    setRotation(getRotation() + m_angularVelocity * elapsedTime);

}

/**
 * @brief This just runs through each force in m_forces and uses the impulse formula F dt = dp to adjust
 * the velocity and angular velocity
 */
void Polygon::applyForces() {

    for (Force f: m_forces) {

        // First adjust the linear velocity
        if (getLinearFreedom()) {
            sf::Vector2f impulse = f.magnitude * f.unitVector * f.impulseTime;
            sf::Vector2f dv = impulse / getMass();
            std::cout << dv.x << " " << dv.y << std::endl;
            m_velocity += dv;
        }

        // Now adjust the torque
        ///*
        if (getRotationalFreedom()) {
            float dw = VectorMath::cross(f.COMVector, f.unitVector * f.magnitude) / getMomentOfInertia() * 5000.0f;
            std::cout << dw << std::endl;
            m_angularVelocity += dw;
        }
        //*/
    }

    // And clear the forces now that they've been applied
    m_forces.clear();
}

///////////////////////////////////////
//          TRANSFORMATIONS
///////////////////////////////////////

/*
The following methods are "overridden" versions of there super class methods, but since it will change
the points we have on our polygon, we need to recreate (or hopefully only update) our lines that
bound the outside
*/

void Polygon::setScale(const sf::Vector2f& scale) {
    Transformable::setScale(scale.x, scale.y);

    m_lineUpdateRequired = true;
}

void Polygon::setScale(float xFactor, float yFactor) {
    Transformable::setScale(xFactor, yFactor);

    m_lineUpdateRequired = true;
}

void Polygon::scale(const sf::Vector2f& scale) {
    Transformable::scale(scale.x, scale.y);

    m_lineUpdateRequired = true;
}

void Polygon::scale(float xFactor, float yFactor) {
    Transformable::scale(xFactor, yFactor);

    m_lineUpdateRequired = true;
}

void Polygon::setRotation(float angle) {
    Transformable::setRotation(angle);

    m_lineUpdateRequired = true;
}

void Polygon::rotate(float angle) {
    Transformable::rotate(angle);

    m_lineUpdateRequired = true;
}

void Polygon::setPosition(const sf::Vector2f& position) {
    Transformable::setPosition(position.x, position.y);

    m_lineUpdateRequired = true;
}

void Polygon::setPosition(float x, float y) {
    Transformable::setPosition(x, y);

    m_lineUpdateRequired = true;
}

void Polygon::move(const sf::Vector2f& d) {
    Transformable::move(d.x, d.y);

    m_lineUpdateRequired = true;
}

void Polygon::move(float dx, float dy) {
    Transformable::move(dx, dy);

    m_lineUpdateRequired = true;
}

void Polygon::setSize(sf::Vector2f size) {

    sf::Vector2f actualSize = sf::Vector2f(getLocalBounds().width, getLocalBounds().height);

    sf::Vector2f scale = sf::Vector2f(size.x / actualSize.x, size.y / actualSize.y);

    Polygon::setScale(scale);

}

void Polygon::setSize(float width, float height) {
    Polygon::setSize(sf::Vector2f(width, height));
}


/**********************************
 * VELOCITY AND MOVEMENT THINGS
**********************************/
/**
 * @brief Returns the current linear velocity of the shape
 * 
 * @return sf::Vector2f Current linear velocity of the shape
 */
sf::Vector2f Polygon::getVelocity() {
    return m_velocity;
}

/**
 * @brief Changes the linear velocity of the polygon to the paramter provided
 * 
 * @param newVelocity The new velocity of the polygon
 */
void Polygon::setVelocity(sf::Vector2f newVelocity) {
    m_velocity = newVelocity;
}

/**
 * @brief Returns the current angular velocity of the polygon to the paramter provided
 * 
 * @return float Current angular velocity of the polygon
 */
float Polygon::getAngularVelocity() {
    return m_angularVelocity;
}

/**
 * @brief Changes the angular velocity of the polygon to the paramter provided
 * 
 * @param newAngularVelocity The new angular velocity of the polygon
 */
void Polygon::setAngularVelocity(float newAngularVelocity) {
    m_angularVelocity = newAngularVelocity;
}
