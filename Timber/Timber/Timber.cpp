// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//include library
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//using namespace
using namespace sf;

// Declare function
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the branch/player
// Left or right
enum class side{LEFT,RIGHT,NONE};
side branchPosition[NUM_BRANCHES];

int main()
{
    //create a video mode object
    VideoMode vm(1920, 1080);

    //create  and open a window for the game RenderWindow
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    //create a texture to hold a graphic on the GPU
    Texture textureBackground;

    //Load a graphic into to texture
    textureBackground.loadFromFile("graphics/background.png");

    //create a sprite
    Sprite spriteBackground;

    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    // Set the spritebackground to cover the screen
    spriteBackground.setPosition(0, 0);

    // Set a tree
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Set a bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);

    // Bee is active?
    bool beeActive = false;

    // How fast can the bee fly;
    float beeSpeed = 0.0f;

    // Set 3 Clouds
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);

    spriteCloud2.setTexture(textureCloud);
    spriteCloud2.setPosition(0, 250);

    spriteCloud3.setTexture(textureCloud);
    spriteCloud3.setPosition(0, 500);

    // 3 Clouds is Active?
    bool cloud1Active, cloud2Active, cloud3Active;
    cloud1Active = cloud2Active = cloud3Active = false;

    // Speed of 3 Clouds
    float cloud1Speed, cloud2Speed, cloud3Speed;
    cloud1Speed = cloud2Speed = cloud3Speed = 0.0f;

    // Set timberman
    Texture textureTimber;
    textureTimber.loadFromFile("graphics/player.png");
    Sprite spriteTimber;
    spriteTimber.setTexture(textureTimber);
    spriteTimber.setPosition(580, 720);

    // the player starts on the left
    side playerSide = side::LEFT;

    // Set the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);
    
    // Set axe
    Texture textureAXE;
    textureAXE.loadFromFile("graphics/axe.png");
    Sprite spriteAXE;
    spriteAXE.setTexture(textureAXE);
    //spriteAXE.setRotation(180);
    spriteAXE.setPosition(700, 830);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Set flying log
    Texture textureLOG;
    textureLOG.loadFromFile("graphics/log.png");
    Sprite spriteLOG;
    spriteLOG.setTexture(textureLOG);
    spriteLOG.setPosition(810, 720);

    // Variable log flying
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Track whether the game is running
    bool paused = true;

    // Draw some text
    int score = 0;
    int lastDrawn = 0;
    float fps = 0.0f;

    sf::Text messageText;
    sf::Text scoreText;
    sf::Text fpsText;

    // Setup font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);
    fpsText.setFont(font);

    // Assign the actual message
    messageText.setString("Press enter to start!");
    scoreText.setString("Score = 0");
    fpsText.setString("");

    // Size of the text
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    fpsText.setCharacterSize(100);

    // Set color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    fpsText.setFillColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);

    fpsText.setPosition(1200, 20);

    // Set 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        // Set the sprite's origin to dead center
        // we can then spin it round without changing its position
        branches[i].setOrigin(220, 20);
    }
    
    updateBranches(1);
    updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);

    // Control the player input
    bool acceptInput = false;

    // Prepare the sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    while (window.isOpen())
    {
        /*
        ****************************************
        Handle the players input
        ****************************************
        */

        Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                // Listen for key presses again
                acceptInput = true;

                // hide the axe
                spriteAXE.setPosition(2000, spriteAXE.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // Start the game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            // Reset the time and the score
            score = 0;
            timeRemaining = 6;

            // Make all the branches disappear
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPosition[i] = side::NONE;
            }

            // Make sure the gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            // Move the player into position
            spriteTimber.setPosition(580, 720);

            acceptInput = true;
            
        }
        // Wrap the player controls to make sure we are accepting input
        if (acceptInput)
        {
            // First handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                // Make sure the player is on the right
                playerSide = side::RIGHT;
                score++;

                // increase time remaining
                timeRemaining += (2 / score) + .15;

                // Move the axe
                spriteAXE.setPosition(AXE_POSITION_RIGHT, spriteAXE.getPosition().y);

                // Move the player
                spriteTimber.setPosition(1200, 720);

                // Update the branches
                updateBranches(score);

                // Set the log flying to the left
                spriteLOG.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                // Play chop sound
                chop.play();
            }
            // Handle the left input
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                // Make sure the player is on the left
                playerSide = side::LEFT;
                score++;

                // increase time remaining
                timeRemaining += (2 / score) + .15;

                // Move the axe
                spriteAXE.setPosition(AXE_POSITION_LEFT, spriteAXE.getPosition().y);

                // Move the player
                spriteTimber.setPosition(580, 720);

                // Update the branches
                updateBranches(score);

                // Set the log flying to the left
                spriteLOG.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                // Play chop sound
                chop.play();
            }
        }

        /*
        ****************************************
        Update the scene
        ****************************************
        */

        if (!paused)
        {
            // Measure time
            Time dt = clock.restart();

            // Decrease time remaining
            timeRemaining -= dt.asSeconds();

            // Resize the time remaining bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            // running out of time
            if (timeRemaining <= 0.0f)
            {
                // pause the game
                paused = true;

                // Change the messsage shown to the player
                messageText.setString("Out of time!!");

                // Restore the remaining bar based on its new size
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // play out of time sound
                outOfTime.play();
            }

            // Setup the bee
            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                // How high is the bee
                srand((int)time(0) * 10);
                // Make the bee on 500+
                float height = (rand() % 500) + 500;
                // Make the bee on 0 - 999
                //float height = (rand() % 1000);
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else // move the bee
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                // Has the bee reached the right hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    // Set it up ready to be a whole new cloud next frame
                    beeActive = false;
                }
            }

            // Setup the first cloud
            if (!cloud1Active)
            {
                // Speed of the cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200) + 50;

                srand((int)time(0) * 10);
                // High of cloud
                float height = rand() % 150;
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else // Move the first cloud
            {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
                // Has the first cloud reached the right edge of the screen?
                if (spriteCloud1.getPosition().x > 1920)
                {
                    // Set it to left edge
                    cloud1Active = false;
                }
            }

            // Setup the second cloud
            if (!cloud2Active)
            {
                // Speed of the cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200) + 100;

                srand((int)time(0) * 20);
                // High of cloud
                float height = (rand() % 350) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else // Move the first cloud
            {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
                // Has the first cloud reached the right edge of the screen?
                if (spriteCloud2.getPosition().x > 1920)
                {
                    // Set it to left edge
                    cloud2Active = false;
                }
            }

            // Setup the third cloud
            if (!cloud3Active)
            {
                // Speed of the cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200) + 100;

                srand((int)time(0) * 30);
                // High of cloud
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else // Move the first cloud
            {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
                // Has the first cloud reached the right edge of the screen?
                if (spriteCloud3.getPosition().x > 1920)
                {
                    // Set it to left edge
                    cloud3Active = false;
                }
            }
            lastDrawn++;
            // Draw fps and the score once for every 100 frames
            if (lastDrawn == 100)
            {
                std::stringstream ss;
                ss << "Score = " << score;
                scoreText.setString(ss.str());

                std::stringstream ss1;
                ss1 << "FPS = " << 1/dt.asSeconds();
                fpsText.setString(ss1.str());
                lastDrawn = 0;
            }

            // Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPosition[i] == side::LEFT)
                {
                    // Move the sprite to the left side
                    branches[i].setPosition(590, height);
                    
                    // Flip the branch
                    branches[i].setRotation(180);
                }
                else if (branchPosition[i] == side::RIGHT)
                {
                    // Move the sprite to the left side
                    branches[i].setPosition(1330, height);

                    // branch in right side
                    branches[i].setRotation(0);
                }
                else
                {
                    // Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }

            // Handle a flying log
            if (logActive)
            {
                spriteLOG.setPosition(spriteLOG.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLOG.getPosition().y + (logSpeedY * dt.asSeconds()));

                // Has the log reached the right hand edge?
                if (spriteLOG.getPosition().x < -100 || spriteLOG.getPosition().x > 2000) 
                {
                    // Set it up ready to be a whole new log next frame
                    logActive = false;
                    spriteLOG.setPosition(810, 720);
                }
            }

            // Handle the death
            if (branchPosition[5] == playerSide)
            {
                // death
                paused = true;
                acceptInput = false;

                // Draw the gravestone
                spriteRIP.setPosition(525, 760);

                // Hide the player
                spriteTimber.setPosition(2000, 660);

                // Change the text of the message
                messageText.setString("SQUISHED!!!");

                // Center the message
                FloatRect textRect = messageText.getLocalBounds();

                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // Play death sound
                death.play();
            }
        }

        /*
        ****************************************
        Draw the scene
        ****************************************
        */
        // Clear everything from the last frame
        window.clear();

        //Draw our game scene here
        window.draw(spriteBackground);

        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        window.draw(spriteTree);

        window.draw(spriteTimber);

        window.draw(spriteAXE);

        window.draw(spriteLOG);

        window.draw(spriteRIP);

        window.draw(spriteBee); 

        window.draw(scoreText);

        window.draw(timeBar);

        window.draw(fpsText);

        if (paused)
        {
            window.draw(messageText);
        }

        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        //show everything we just draw
        window.display();
    }
    return 0;
}

// Function update branches
void updateBranches(int seed)
{
    // Move all the branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPosition[j] = branchPosition[j - 1];
    }

    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r)
    {
    case 0 :
        branchPosition[0] = side::LEFT;
        break;

    case 1 :
        branchPosition[0] = side::RIGHT;
        break;

    default :
        branchPosition[0] = side::NONE;
        break;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
