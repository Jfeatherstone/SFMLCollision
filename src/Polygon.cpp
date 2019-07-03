#include "Polygon.hpp"

/**
 * @brief Construct a new Polygon object from a given texture (image).
 * 
 * @param texture The texture for the shape/sprite we want to model
 * @param detail The level of detail to keep in the shape, from least to most: Less, More, Optimal, Exact
 * @param ignoredsf::Colors By default, all pixels that arent (0, 0, 0, 0) will be included, any colors specified here will also be ignored
 */
Polygon::Polygon(sf::Texture* texture, Detail detail, std::vector<sf::Color> ignoredColors) {


    // First we store the texture such that the polygon looks like the image
    //setTexture(texture);

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

    // Separate out each color and create an object to add to our std::vector
    for (int i = 0; i < length; i++) {
        int red = (int) arr[4*i];
        int green = (int) arr[4*i + 1];
        int blue = (int) arr[4*i + 2];
        int alpha = (int) arr[4*i + 3];
        sf::Color c(red, green, blue, alpha);
        pixels[i] = c;
    }

    ///////////////////////////////////////////
    //     Create our first set of verticies
    ///////////////////////////////////////////
    std::vector<int> hitboxInclude;
    // Next, we want to go through every color and if it isn't empty or on the ignore list, we
    // add it to our include for the hitbox calculation
    hitboxInclude.resize(pixels.size());

    sf::Vector2f textureSize;

    textureSize.x = texture->getSize().x;
    textureSize.y = texture->getSize().y;

    int i = 0;

    for (sf::Color c: pixels) {
        if (!contains(ignoredColors, c) && (int) c.a > 0) {
            //cout << (int)c.r << " " << (int)c.g << " " << (int)c.b << " " << (int)c.a << endl;
            hitboxInclude[i] = 1;
        }
        else
            hitboxInclude[i] = 0;
        i++;
    }

    /*
    ///////////////////////////////////////////////
    // Print out our current verticies to help debug
    cout << endl;
    for (int i = 0; i < hitboxInclude.size(); i++) {
        if (i % (int)(textureSize.x) == 0 && i > 0)
            cout << endl;
        cout << hitboxInclude[i];

    }
    cout << "\n\n";
    ///////////////////////////////////////////////
    */


    ///////////////////////////////////////////
    //     Fill in the inside
    ///////////////////////////////////////////
    /*
    We want to check, for every point that has a value of 0, whether it is bounded on all four sides as well as on the diagonals
        */
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            if (hitboxInclude[i*textureSize.x + j] == 0) {
                bool right = false, left = false, up = false, down = false; // Our cardinal directions
                bool upright = false, downright = false, upleft = false, downleft = false; // We also want to check diagonals
                // Right
                for (int k = j+1; k < textureSize.x; k++) {
                    if (hitboxInclude[i*textureSize.x + k] == 1) {
                        right = true;
                        break;
                    }
                }
                // Left
                for (int k = j-1; k >= 0; k--) {
                    if (hitboxInclude[i*textureSize.x + k] == 1) {
                        left = true;
                        break;
                    }
                }
                // Up
                for (int k = i-1; k >= 0; k--) {
                    if (hitboxInclude[k*textureSize.x + j] == 1) {
                        up = true;
                        break;
                    }
                }
                // Down
                for (int k = i+1; k < textureSize.y; k++) {
                    if (hitboxInclude[k*textureSize.x + j] == 1) {
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
                    
                    if (hitboxInclude[(i-k)*textureSize.x + j + k] == 1) {
                        upright = true;
                        break;
                    }
                }
                // Down right
                for (int k = 0; k < textureSize.x + textureSize.y; k++) {
                    // We also will have another if as to optimize our code
                    if (i+k > textureSize.y || j + k > textureSize.x)
                        break;
                    
                    if (hitboxInclude[(i+k)*textureSize.x + j + k] == 1) {
                        downright = true;
                        break;
                    }
                }
                // Down left
                for (int k = 0; k < textureSize.x + textureSize.y; k++) {
                    // We also will have another if as to optimize our code
                    if (i+k > textureSize.y || j - k < 0)
                        break;
                    
                    if (hitboxInclude[(i+k)*textureSize.x + j - k] == 1) {
                        downleft = true;
                        break;
                    }
                }
                // Up left
                for (int k = 0; k < textureSize.x + textureSize.y; k++) {
                    // We also will have another if as to optimize our code
                    if (i-k < 0 || j - k < 0)
                        break;
                    
                    if (hitboxInclude[(i-k)*textureSize.x + j - k] == 1) {
                        upleft = true;
                        break;
                    }
                }

                if (left && right && up && down && upleft && upright && downleft && downright) {
                    hitboxInclude[i*textureSize.x + j] = 1;
                }
            }
        }
    } 

    ///*
    ///////////////////////////////////////////////
    // Print out our current verticies to help debug
    std::cout << std::endl;
    for (int i = 0; i < hitboxInclude.size(); i++) {
        if (i % (int)(textureSize.x) == 0 && i > 0)
            std::cout << std::endl;
        std::cout << hitboxInclude[i];

    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    ///////////////////////////////////////////
    //     Removing the inside
    ///////////////////////////////////////////
    // Now, we go through and remove everything except for the outline
    std::vector<int> newHitbox;
    newHitbox.resize(hitboxInclude.size());

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

            Lastly, we make sure that we aren't at the bottom line. This was causing an error where some
            on the last line weren't included and I have no idea why, but this seemed like a good way
            to fix it :)
            */
            newHitbox[i*textureSize.x + j] = hitboxInclude[i*textureSize.x + j];

            if ((hitboxInclude[i*textureSize.x + j - 1] == 1 && i*textureSize.x + j - 1 >= i*textureSize.x)
            && (hitboxInclude[i*textureSize.x + j + 1] == 1 && i*textureSize.x + j + 1 < (i+1)*textureSize.x)
            && (hitboxInclude[(i-1)*textureSize.x + j] == 1) && (hitboxInclude[(i+1)*textureSize.x + j] == 1)
            && ((i+1)*textureSize.x + j < hitboxInclude.size() && (i-1) >= 0)
            && (hitboxInclude[(i-1)*textureSize.x + j - 1] == 1) && (hitboxInclude[(i-1)*textureSize.x + j + 1] == 1)
            && (hitboxInclude[(i+1)*textureSize.x + j - 1] == 1) && (hitboxInclude[(i+1)*textureSize.x + j + 1] == 1))            
                newHitbox[i*textureSize.x + j] = 2;
        }
    }
    hitboxInclude = newHitbox;

    ///*
    ///////////////////////////////////////////////
    // Print out our current verticies to help debug
    std::cout << std::endl;
    for (int i = 0; i < hitboxInclude.size(); i++) {
        if (i % (int)(textureSize.x) == 0 && i > 0)
            std::cout << std::endl;
        std::cout << hitboxInclude[i];

    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    /*****************************************
     *      Remove excess verticies
     * ******************************************/
    // The final process is to remove points in between straight sections, which would cause
    // uneccessary points on our hitbox
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {
            
            // We don't change inside points
            if (hitboxInclude[i*textureSize.x + j] == 2)
                continue;

            // This just checks that both the left and right pixels are either on a different line or are included.
            if ((hitboxInclude[i*textureSize.x + j - 1] == 1 && i*textureSize.x + j - 1 >= i*textureSize.x)
            && (hitboxInclude[i*textureSize.x + j + 1] == 1 && i*textureSize.x + j + 1 < (i+1)*textureSize.x))
                newHitbox[i*textureSize.x + j] = 3;
            
            // Now we do the same, but vertically
            if ((hitboxInclude[(i-1)*textureSize.x + j] == 1 && (i-1) >= 0)
            && (hitboxInclude[(i+1)*textureSize.x + j] == 1 && (i+1) < textureSize.y))
                newHitbox[i*textureSize.x + j] = 3;

        }
    }
    hitboxInclude = newHitbox;

    ///*
    ///////////////////////////////////////////////
    // Print out our current verticies to help debug
    std::cout << std::endl;
    for (int i = 0; i < hitboxInclude.size(); i++) {
        if (i % (int)(textureSize.x) == 0 && i > 0)
            std::cout << std::endl;
        std::cout << hitboxInclude[i];

    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    /*
    Remove diagonal verticies

    What this method  does:

    001     001
    011 ->  010
    110     100

    It removes the "squareness" of diagonal lines by removing extra verticies
    */
    for (int i = 0; i < textureSize.y; i++) {
        for (int j = 0; j < textureSize.x; j++) {

            // Top right diagonal
            if (hitboxInclude[i*textureSize.x + j] == 1 // The current point is a vertex
            && hitboxInclude[(i-1)*textureSize.x + j + 1] == 1 // The top right point is a vertex and in our std::vector
            && (i-1) >= 0 && j + 1 < textureSize.x) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (hitboxInclude[i*textureSize.x + j + 1] == 1)
                    hitboxInclude[i*textureSize.x + j + 1] = 0; // right
                if (hitboxInclude[(i-1)*textureSize.x + j] == 1) 
                    hitboxInclude[(i-1)*textureSize.x + j] = 0; // above
            }

            // Bottom right diagonal
            if (hitboxInclude[i*textureSize.x + j] == 1 // The current point is a vertex
            && hitboxInclude[(i+1)*textureSize.x + j + 1] == 1 // The bottom right point is a vertex
            && (i+1)*textureSize.x + j < hitboxInclude.size() && j + 1 < textureSize.x) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (hitboxInclude[i*textureSize.x + j + 1] == 1)
                    hitboxInclude[i*textureSize.x + j + 1] = 0; // right
                if (hitboxInclude[(i+1)*textureSize.x + j] == 1) 
                    hitboxInclude[(i+1)*textureSize.x + j] = 0; // below
            }

            // Bottom left diagonal
            if (hitboxInclude[i*textureSize.x + j] == 1 // The current point is a vertex
            && hitboxInclude[(i+1)*textureSize.x + j - 1] == 1 // The bottom left point is a vertex
            && (i+1)*textureSize.x + j < hitboxInclude.size() && j - 1 >= 0) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (hitboxInclude[i*textureSize.x + j - 1] == 1)
                    hitboxInclude[i*textureSize.x + j - 1] = 0; // left
                if (hitboxInclude[(i+1)*textureSize.x + j] == 1) 
                    hitboxInclude[(i+1)*textureSize.x + j] = 0; // below
            }

            // Top left diagonal
            if (hitboxInclude[i*textureSize.x + j] == 1 // The current point is a vertex
            && hitboxInclude[(i-1)*textureSize.x + j - 1] == 1 // The top right point is a vertex
            && (i-1)*textureSize.x > 0 && j + 1 < textureSize.x) { // Make sure we are within bounds
            // We want to make sure that the vertex was actually a point before (might be unnecessary)
                if (hitboxInclude[i*textureSize.x + j - 1] == 1)
                    hitboxInclude[i*textureSize.x + j - 1] = 0; // left
                if (hitboxInclude[(i-1)*textureSize.x + j] == 1) 
                    hitboxInclude[(i-1)*textureSize.x + j] = 0; // above
            }

        }
    }

    ///*
    ///////////////////////////////////////////////
    // Print out our current verticies to help debug
    std::cout << std::endl;
    for (int i = 0; i < hitboxInclude.size(); i++) {
        if (i % (int)(textureSize.x) == 0 && i > 0)
            std::cout << std::endl;
        std::cout << hitboxInclude[i];

    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    /*
    Remove diagonal verticies part 2 - Lines

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
            if (hitboxInclude[i*textureSize.x + j] == 1
            && (hitboxInclude[(i-1)*textureSize.x + j - 1] == 1 || hitboxInclude[(i-1)*textureSize.x + j - 1] == 3)
            && (hitboxInclude[(i+1)*textureSize.x + j + 1] == 1 || hitboxInclude[(i+1)*textureSize.x + j + 1] == 3)
            && (hitboxInclude[(i)*textureSize.x + j + 1] != 1 && hitboxInclude[(i)*textureSize.x + j - 1] != 1
            && hitboxInclude[(i+1)*textureSize.x + j] != 1 && hitboxInclude[(i-1)*textureSize.x + j] != 1))
                hitboxInclude[i*textureSize.x + j] = 3;

            if (hitboxInclude[i*textureSize.x + j] == 1
            && (hitboxInclude[(i+1)*textureSize.x + j - 1] == 1 || hitboxInclude[(i+1)*textureSize.x + j - 1] == 3)
            && (hitboxInclude[(i-1)*textureSize.x + j + 1] == 1 || hitboxInclude[(i-1)*textureSize.x + j + 1] == 3)
            && (hitboxInclude[(i)*textureSize.x + j + 1] != 1 && hitboxInclude[(i)*textureSize.x + j - 1] != 1
            && hitboxInclude[(i+1)*textureSize.x + j] != 1 && hitboxInclude[(i-1)*textureSize.x + j] != 1))
                hitboxInclude[i*textureSize.x + j] = 3;

        }
    }

    ///*
    ///////////////////////////////////////////////
    // Print out our current verticies to help debug
    std::cout << std::endl;
    for (int i = 0; i < hitboxInclude.size(); i++) {
        if (i % (int)(textureSize.x) == 0 && i > 0)
            std::cout << std::endl;
        std::cout << hitboxInclude[i];

    }
    std::cout << "\n\n";
    ///////////////////////////////////////////////
    //*/

    //////////////////////////////////////////
    //      Add verticies in order
    //////////////////////////////////////////
    /*
    Crunch time!
    We now have every pixel that will represent a vertex in our hitbox polygon, the problem is
    now to put them in order so that it makes a corherent shape.

    We can't just go across and add each one, because then the shape would be zig-zagged and not
    work.

    We need to find a way to assign each pixel in the outline by following the path around our
    shape.

    Since we have an outline of the sprite (where the lines should go) indicated by a 3 in our std::vector,
    we should be able to begin at a given pixel and move along in one direction
    */
    sf::Vector2f currPixel;
    int vertexIndex = 0;
    std::vector<sf::Vector2f> hitboxVerticies;
    hitboxVerticies.resize(hitboxInclude.size()); // This is a little overkill, but that's okay

    // Setup our polygon

    // We need to know how many verticies we have
    m_numVerticies = 0;
    for (int i: hitboxInclude) {
        if (i == 1)
            m_numVerticies++;

    }
    m_points.resize(m_numVerticies);

    std::cout << m_numVerticies << std::endl;

    sf::Vector2f previousMovement(0, 0);

    while (vertexIndex < m_numVerticies) {
        if (hitboxInclude[currPixel.y*textureSize.x + currPixel.x] == 1 || hitboxInclude[currPixel.y*textureSize.x + currPixel.x] == 3) {
            // Even if it isn't an actual vertex, we record it in our other std::vector
            hitboxVerticies.push_back(currPixel);
            std::cout << currPixel.x << " " << currPixel.y;

            if (hitboxInclude[currPixel.y*textureSize.x + currPixel.x] == 1) {
                // We record the vertex in our polygon
                std::cout << " - Added " << vertexIndex << std::endl;
                m_points[vertexIndex++] = currPixel;
            } else {
                std::cout << std::endl;
            }
            // We now look for the next pixel that is marked either as a 3 or a 1
            /*
            We check starting at the top and move clockwise 
            */
            //cout << currPixel.x << " " << currPixel.y << " " << vertexIndex << endl;
            //for (sf::Vector2f v: m_hitboxVertices)
              //  cout << v.x << " " << v.y << " --- ";

            // First check the direction we moved in last
            if (previousMovement != sf::Vector2f(0, 0)) {
                sf::Vector2f newPixel = currPixel + previousMovement;

                if ((hitboxInclude[(newPixel.y)*textureSize.x + newPixel.x] == 1 
                || hitboxInclude[(newPixel.y)*textureSize.x + newPixel.x] == 3)
                && ((newPixel.x) >= 0 && newPixel.x < textureSize.x && (newPixel.y)*textureSize.x + newPixel.x < hitboxInclude.size())
                && !contains(hitboxVerticies, newPixel)) {
                    currPixel = newPixel;
                    continue;
                }
            }

            // Top
            if ((hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x] == 1 
            || hitboxInclude[(currPixel.y - 1)*textureSize.x + currPixel.x] == 3)
            && ((currPixel.y - 1) >= 0 && (currPixel.y - 1)*textureSize.x + currPixel.x < hitboxInclude.size())
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x, currPixel.y - 1))) {
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
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x + 1, currPixel.y - 1))) {
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
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x + 1, currPixel.y))) {
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
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x + 1, currPixel.y + 1))) {
                currPixel.x += 1;
                currPixel.y += 1;
                previousMovement = sf::Vector2f(1, 1);
                continue;
            } else
            // Bottom
            if ((hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x] == 1 
            || hitboxInclude[(currPixel.y + 1)*textureSize.x + currPixel.x] == 3)
            && ((currPixel.y + 1)*textureSize.x + currPixel.x >= 0 && (currPixel.y + 1)*textureSize.x + currPixel.x < hitboxInclude.size())
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x, currPixel.y + 1))) {
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
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x - 1, currPixel.y + 1))) {
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
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x - 1, currPixel.y))) {
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
            && !contains(hitboxVerticies, sf::Vector2f(currPixel.x - 1, currPixel.y - 1))) {
                currPixel.x += -1;
                currPixel.y += -1;
                previousMovement = sf::Vector2f(-1, -1);
                continue;
            } else {
                //cout << "Done" << endl;
                break;
            }
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

    std::cout << vertexIndex << std::endl;

    ///*
    // 0, 0 has been causing some trouble, so we remove it if it isn't actually there
    if (hitboxInclude[0] != 1) {
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

    // Update the size of our verticies
    m_numVerticies = m_points.size();

    //////////////////////////////////////////
    // 
    // END OF SHAPE GENERATION
    // 
    // It's over. It's done.
    //
    // I sincerely apologize for that mess
    //////////////////////////////////////////
    
    /*
    Area optimization
    TODO: Make this better
    */
    // We only run this if we are trying to optimize verticies (i.e. not Exact detail)
    if (detail != Detail::Exact) {
        // Depending on our level of detail, we have a higher or lower tolerance
        // for the change in the area when removing points
        float diff = .05f;
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
                Polygon::getArea(m_points, a1);
                Polygon::getArea(old, a2);

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

    m_numVerticies = m_points.size();

    findCentroid();
    createLines();
    calculateMass();
    Shape::update(); // This makes the shape actually drawable
}

/////////////////////////////////////////////////////////////
//    The following methods are used in the above constructor
/////////////////////////////////////////////////////////////

/**
 * @brief Checks whether a color is contained within a std::vector of colors
 * 
 * @param vec A std::vector of colors
 * @param c A specific color
 * @return true If vec contains c
 * @return false If vec doesn't contain c
 */
bool Polygon::contains(std::vector<sf::Color>& vec, sf::Color c) {
    /*
    We use this method in determining whether a list of colors to be ignored was provided
    As we read every pixel from an image, we check whether the ignoredsf::Colors std::vector
    contains the rgba value
    */
    for (sf::Color col: vec) {
        if (col == c)
            return true;
    }
    return false;
}

/**
 * @brief Checks whether a point is contained within a std::vector of points
 * 
 * @param vec A std::vector of points
 * @param point A specific point
 * @return true If vec contains point
 * @return false If vec doesn't contain point
 */
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

/**
 * @brief Construct a new Polygon object from a std::vector of points
 * 
 * @param points The points that constitute our shape
 */
Polygon::Polygon(std::vector<sf::Vector2f> points) {
    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createLines();
    calculateMass();
    Shape::update(); // This makes the shape actually drawable
}

/**
 * @brief Construct a new Polygon object from a sf::CircleShape object
 * 
 * @param shape The CircleShape object whose points we will use
 */
Polygon::Polygon(sf::CircleShape shape) {
    std::vector<sf::Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createLines();
    calculateMass();
    Shape::update(); // This makes the shape actually drawable
}

/**
 * @brief Construct a new Polygon object from a sf::RectangleShape object
 * 
 * @param shape The RectangleShape object whose points we will use
 */
Polygon::Polygon(sf::RectangleShape shape) {
    std::vector<sf::Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createLines();
    calculateMass();
    Shape::update(); // This makes the shape actually drawable
}

/**
 * @brief Construct a new Polygon object from a sf::ConvexShape object
 * 
 * @param shape The ConvexShape object who points we will use
 */
Polygon::Polygon(sf::ConvexShape shape) {
    std::vector<sf::Vector2f> points;
    points.resize(shape.getPointCount());
    for (int i = 0; i < shape.getPointCount(); i++) {
        points[i] = shape.getPoint(i);
    }

    m_points = points;

    m_numVerticies = m_points.size();

    findCentroid();
    createLines();
    calculateMass();
    Shape::update(); // This makes the shape actually drawable
}

/**
 * @brief Using our current m_points, we recreate the lines that represent the boundary of our
 * shape. This is called whenever our shape is transformed (moved, rotated, scaled)
 * 
 */
void Polygon::createLines() {
    //cout << "Creating Lines" << endl;
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
    for (int i = 0; i < m_numVerticies; i++) {
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
    for (int i = 0; i < m_numVerticies; i++) {
        m_points[i].x += offset.x;
        m_points[i].y += offset.y; 
    }

    /*
    Now that our points properly represent the shape we want, we can create lines between them to check for collisions
    */
    m_lines.clear();
    m_lines.resize(m_numVerticies);
    
    for (int i = 0; i < m_points.size() - 1; i++) {
        m_lines[i] = Line(m_points[i], m_points[i+1]);
    }

    m_lines[m_numVerticies - 1] = Line(m_points[m_numVerticies - 1], m_points[0]);

    m_points = pointsCopy;
}

/**
 * @brief Return the lines that represent the polygon's outline/border
 * 
 * @return std::vector<Line> A std::vector of lines that represent the outline
 */
std::vector<Line> Polygon::getLines() {
    //createLines();
    return m_lines;
}


/**
 * @brief Calculate the centroid of our object by finding the rightmost, leftmost, topmost, etc. points
 * and taking the average of them all. Should give about the same as taking half of each getGlobalBounds()
 * width and height (for setting the origin). Also calculates the farthest vertex distance from the centroid
 */
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

    float previousFarthest = 0;

    // Now, we record the farthest vertex from the centroid
    for (int i = 0; i < m_numVerticies; i++) {
        float d = sqrt(pow(m_centroid.x - m_points[i].x , 2) + pow(m_centroid.y - m_points[i].y, 2));
        if (d > previousFarthest) {
            previousFarthest = d;
            m_farthestVertex.x = m_centroid.x - m_points[i].x;
            m_farthestVertex.y = m_centroid.y - m_points[i].y;
        }
    }
}

/**
 * @brief Get the number of verticies on our polygon
 * 
 * @return size_t The number of verticies
 */
size_t Polygon::getPointCount() const {
    return m_numVerticies;
}

/**
 * @brief Get the vertex at index in the std::vector m_points
 * 
 * @param index The index of the point we are looking for
 * @return sf::Vector2f The point at index in m_points
 */
sf::Vector2f Polygon::getPoint(size_t index) const {
    return m_points[index];
}

/**
 * @brief Returns the entire std::vector of points that represent the shape, without any modifications from
 * transformations (rotate, move, scale)
 * 
 * @return std::vector<sf::Vector2f> Our shape's std::vector of verticies
 */
std::vector<sf::Vector2f> Polygon::getPoints() {
    return m_points;
}

/**
 * @brief Return the area of the polygon
 * 
 * @return float The area of the polygon
 */
float Polygon::getArea() {
     return m_area;
}

/**
 * @brief Calculate the mass of the polygon using the area and density
 * 
 */
void Polygon::calculateMass() {
    Polygon::getArea(getPoints(), m_area);
    m_mass = m_density * m_area;
}

/**
 * @brief Calculate the (relative) moment of inertia of the object by using the distance from the
 * centroid to every vertex on the boundary. Only has value when comparing moment of inertia's between
 * shapes, doesn't give the actual moment of inertia of a real object.
 * 
 * Todo: Make this use the average point location times the mass
 * 
 */
void Polygon::calculateMomentOfInertia() {
    // We assume a uniform distribution of density throughout the object
    // given by m_density

    /*
    I = \int{r^2 dm}
    Instead of using every point in the shape, we instead just use the verticies. This won't give us the exact moment of inertia
    in a physical sense, but will give a value that is proportional to it, making all relative comparisons between shapes accurate

    This makes the calculation super easy, as we just add up the distance from the origin to the points\
    */
    m_centerOfMass = sf::Vector2f(0, 0);
    m_momentOfInertia = 0;

    for (sf::Vector2f p: m_points) {
        m_centerOfMass += p;
    }

    m_centerOfMass.x /= getPointCount();
    m_centerOfMass.y /= getPointCount();

    // Now find the average distance from the center of mass to the points
    for (sf::Vector2f p: m_points) {
        m_momentOfInertia += VectorMath::mag(m_centerOfMass - p);
    }

    m_momentOfInertia /= getPointCount();
    m_momentOfInertia *= m_momentOfInertia * getMass();

}

sf::Vector2f Polygon::getCenterOfMass() {
    return m_centerOfMass;
}

/**
 * @brief Set whether the shape is solid (can collide with other shapes)
 * 
 * @param state Whether or not the shape is solid
 */
void Polygon::setSolid(bool state) {
    m_isSolid = state;
}

/**
 * @brief Check whether or not the shape can collide with other shapes
 * 
 * @return true Can collide
 * @return false Cannot collide
 */
bool Polygon::isSolid() {
    return m_isSolid;
}

/**
 * @brief Set how much energy is conserved when this object collides with another. 0 for no energy conserved
 * (completely inelastic collision) and 1 for completely elastic (all energy conserved)
 * 
 * @param value The new rigidity, 0 for complete inelastic, 1 for complete elastic
 */
void Polygon::setRigidity(float value) {
    m_rigidity = value;
}

/**
 * @brief Get how much energy is conserved when this object collides with another. 0 for no energy conserved
 * (completely inelastic collision) and 1 for completely elastic (all energy conserved)
 * 
 * @return float The rigidity, 0 for complete inelastic, 1 for complete elastic
 */
float Polygon::getRigidity() {
    return m_rigidity;
}

/**
 * @brief Set whether the shape can be moved by being collided with by another object
 * 
 * @param value Whether or not the shape can be moved by another polygon
 */
void Polygon::setMovableByCollision(bool value) {
    m_moveableByCollision = value;
}

/**
 * @brief This will return the parameter that describes how the polygon responds to forces
 * and how it bends and distorts
 * 
 * @return float The Young's Modulus of the shape, default is 1
 */
float Polygon::getYoungsModulus() {
    return m_youngsModulus;
}

/**
 * @brief Change the parameter that describes how the polygon responds to forces and how it
 * bends and distorts
 * 
 * @param youngsModulus The new value for the young's modulus, default is 1
 */
void Polygon::setYoungsModulus(float youngsModulus) {
    m_youngsModulus = youngsModulus;
}

float Polygon::getGamma() {
    return m_gamma;
}

void Polygon::setGamma(float gamma) {
    m_gamma = gamma;
}

sf::Vector2f Polygon::getForce() {
    return m_force;
}

void Polygon::addForce(sf::Vector2f force) {
    m_force += force;
}

void Polygon::setForce(sf::Vector2f force) {
    m_force = force;
}

/**
 * @brief Get whether the shape can be moved by being collided with by another object
 * 
 * @return true The shape can be moved
 * @return false The shape cannot be moved
 */
bool Polygon::isMovableByCollision() {
    return m_moveableByCollision;
}

/**
 * @brief Set the density of the object, used in calculate its mass and moment of inertia (default is 1)
 * and recalculate both values
 * 
 * @param newDensity The density of the object (default is 1)
 */
void Polygon::setDensity(float newDensity) {
    m_density = newDensity;
    // Now recalculate the mass
    calculateMass();
    calculateMomentOfInertia();
}

/**
 * @brief Get the relative density of the polygon
 * 
 * @return float The density of the polygon
 */
float Polygon::getDensity() {
    return m_density;
}

/**
 * @brief Return the mass of the polygon, using the density and area to calculate
 * 
 * @return float The mass of the shape
 */
float Polygon::getMass() {
    return m_mass;
}

/**
 * @brief Return the moment of inertia of the polygon, using the density and vertex distribution
 * 
 * @return float The moment of inertia of the shape
 */
float Polygon::getMomentOfInertia() {
    return m_momentOfInertia;
}

/**
 * @brief This is a static method that finds the area of any given shape (std::vector of points)
 * Ngl, I don't remember where I found this method for finding the area of a polygon, but 
 * will post when I find it. 
 * 
 * @param points A Vector of points the represent our shape. See Polygon::getPoints()
 * @param value A referenced float that our area will be stored in
 */
void Polygon::getArea(std::vector<sf::Vector2f> points, float& value) {
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

/**
 * @brief Returns the distance of the farthest vertex from the centroid. Calculated in findCentroid()
 * 
 * @return float The farthest distance of the shape
 */
float Polygon::getFarthestVertex() {
    /*
    Since we use this method to detect collisions, we also want to update the lines before we do calculations
    with them. This is done through the createLines() method, which accounts for rotation and scale.
    */
    return sqrt(pow(m_farthestVertex.x * getScale().x, 2) + pow(m_farthestVertex.y * getScale().y, 2));
}

/**
 * @brief Returns the centroid of the shape (does not recalculate it)
 * 
 * @return sf::Vector2f The centroid of the shape
 */
sf::Vector2f Polygon::getCentroid() {
    return m_centroid;
}

/*
The following methods are "overridden" versions of there super class methods, but since it will change
the points we have on our polygon, we need to recreate (or hopefully only update) our lines that
bound the outside
*/

/**
 * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param scale The scaling factors for our polygon
 */
void Polygon::setScale(const sf::Vector2f& scale) {
    Transformable::setScale(scale.x, scale.y);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param xFactor The x scaling factor
 * @param yFactor The y scaling factor
 */
void Polygon::setScale(float xFactor, float yFactor) {
    Transformable::setScale(xFactor, yFactor);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param scale The scaling factors for our polygon
 */
void Polygon::scale(const sf::Vector2f& scale) {
    Transformable::scale(scale.x, scale.y);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the scale like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param xFactor The x scaling factor
 * @param yFactor The y scaling factor
 */
void Polygon::scale(float xFactor, float yFactor) {
    Transformable::scale(xFactor, yFactor);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the rotation like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param angle The angle we are setting the rotation to (default is 0)
 */
void Polygon::setRotation(float angle) {
    Transformable::setRotation(angle);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the rotation like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param angle The angle we are rotating the shape by
 */
void Polygon::rotate(float angle) {
    Transformable::rotate(angle);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param position The new x and y coordinates of the shape
 */
void Polygon::setPosition(const sf::Vector2f& position) {
    Transformable::setPosition(position.x, position.y);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param x New x coordinate
 * @param y New y coordinate
 */
void Polygon::setPosition(float x, float y) {
    Transformable::setPosition(x, y);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param d The amount to change x and y by 
 */
void Polygon::move(const sf::Vector2f& d) {
    Transformable::move(d.x, d.y);

    createLines();
}

/**
 * @brief An overriden method from sf::Shape that changes the position like its super-counterpart
 * and also recreates the lines that represent the shape.
 * 
 * @param dx Amount to change the x coordinate by
 * @param dy Amount to change the y coordinate by
 */
void Polygon::move(float dx, float dy) {
    Transformable::move(dx, dy);

    createLines();
}


/**********************************
 * VELOCITY AND MOVEMENT THINGS
**********************************/

/**
 * @brief Updates the shape and applies both linear and angular velocity to update the
 * position and rotation of the polygon
 * 
 * @param elapsedTime The amount of time that has elapsed since the last update
 */
void Polygon::update(float elapsedTime) {
    // Update the position
    setPosition(getPosition() + m_velocity * elapsedTime);
    setRotation(getRotation() + m_angularVelocity * elapsedTime);

    if (VectorMath::mag(m_velocity) <= VELOCITY_THRESHOLD) {
        m_velocity = sf::Vector2f(0, 0);
    }
}

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