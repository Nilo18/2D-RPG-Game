#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include "nature.h"
#include "characters.h"
using namespace sf;
using namespace std;
using namespace std::filesystem;
const float moveDelay = 0.03f;
const float npcTalkDuration = 5.f;
bool npcShouldTalk = false;
VideoMode desktop = VideoMode::getDesktopMode();
GrassGroup grassBlocks("assets/grass2.png", 0, 0, desktop.height / TILE_SIZE + 1, desktop.width / TILE_SIZE + 1); // Globally declared grass block
 
void LetterBoxView(sf::RenderWindow& window, sf::View& view, int wWidth, int wHeight) {
    float windowRatio = (float)wWidth / wHeight; // Ratio of the window
    float viewRatio = view.getSize().x / view.getSize().y; // Ratio of the view
    float sizeX = 1.f, sizeY = 1.f; // Spaces to cover horizontally and vertically
    float posX = 0.f, posY = 0.f; // Starting positions

    // If windowRatio is greater than the viewRatio, that means the width of the window has increased and caused the ratio to become bigger
    if (windowRatio > viewRatio)
    {
        // In this case space to cover horizontally becomes viewRatio / windowRatio to keep the aspect ratio correct
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX)/2.f; // Also, the position should be centered horizontally by subtracting the new space to cover from the whole horizontal space / 2
    }
    // If the viewRatio > windowRatio this means that the windowRatio has shrunk because height has increased
    else {
        // In this case space to cover vertically becomes windowRatio / viewRatio to keep the aspect ratio correct
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY)/2.f; // The position should be centered vertically
    }
    // The viewport should be set to the adjusted sizes and positions
    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    window.setView(view); // The view should be set to the adjusted view
}
void toggleFullscreen(sf::RenderWindow& window, bool& fullscreen, sf::View& view) {
    fullscreen = !fullscreen; // This line makes sure that both setting full screen and exiting it are run on the same button (F11)
    sf::Vector2u windowedSize(990, 880);  // Basic size of window

    VideoMode desktop = VideoMode::getDesktopMode();
    // Close the window because we can't set fullscreen while it is open
    window.close();
    // If the window is in full screen, use the desktop's resolution in fullscreen mode
    if (fullscreen)
        window.create(desktop, "SFML", sf::Style::Fullscreen);
    // If it is not, use the base resolution
    else
        window.create(sf::VideoMode(windowedSize.x, windowedSize.y), "SFML", sf::Style::Default);
    
    Vector2u currentSize = window.getSize();
    view.setSize(static_cast<float>(currentSize.x), static_cast<float>(currentSize.y)); // Set the size of the view to match the base size
    view.setCenter(currentSize.x / 2.f, currentSize.y / 2.f); // Center the view
    window.setView(view); // Set the adjusted view

    // Now the rows and columns to span will change because the screen size has changed
    // This will regenerate the tiles according to the changed rows and columns to span
    grassBlocks.regenerateTiles("assets/grass2.png", 0, 0, currentSize.y / TILE_SIZE + 1, currentSize.x / TILE_SIZE + 1);

    // Adjusts the viewport rectangle in normalized window space to preserve the aspect ratio and add black bars if necessary. 
    // It doesn’t move the camera; it changes how the view is mapped to the physical window.
    //LetterBoxView(window, view, window.getSize().x, window.getSize().y);
}
int main() {
    // Create a window with title and size
    //std::cout << "Working directory is: " << filesystem::current_path() << std::endl; 
    //sf::Vector2u basesize = { 990,800 };
    //VideoMode desktop = VideoMode::getDesktopMode();
    RenderWindow window(desktop, "SFML Test Window", Style::Fullscreen);
    //window.setVisible(false);
    View view;
    view.setSize(static_cast<float>(desktop.width), static_cast<float>(desktop.height));
    view.setCenter(desktop.width / 2.f, desktop.height / 2.f);
    window.setView(view);

    bool fullscreen = true;
     
    // To determine how many blocks we have to draw we divide the width and height by the tile size and add 1
    // First value of Grass/Water Group takes the texture, second and third starting x and y positions and fourth and fifth the area on which they should span
    //grassBlocks = GrassGroup("assets/grass2.png", 0, 0, window.getSize().y / TILE_SIZE + 1, window.getSize().x / TILE_SIZE + 1);
    WaterGroup waterBlocks("assets/water7.png", 0, 0, 3, 5);
    Rock rock("assets/rock2.png", 500, 500);
    Human soldier("assets/avtandila.png", 100, 250);
    NPC npc("assets/avtandila.png", 500, 0);
    npc.setScale(2.f, 2.f);
    soldier.setScale(2.f, 2.f);  // doubles width and height

    Clock moveTime;
    FloatRect bounds = rock.getCollisionBox();

    CircleHitbox hitbox = rock.getCollisionBoxData();

    CircleShape debugBox;
    debugBox.setRadius(hitbox.radius);
    debugBox.setOrigin(hitbox.radius, hitbox.radius);
    debugBox.setPosition(hitbox.centerX, hitbox.centerY);
    debugBox.setFillColor(Color::Transparent);
    debugBox.setOutlineColor(Color::Red);
    debugBox.setOutlineThickness(1.f); // Required to see the red outline

    sf::FloatRect soldierBounds = soldier.getCollisionBox();  // Or next position with offset
    sf::RectangleShape soldierBox;
    soldierBox.setPosition(soldierBounds.left, soldierBounds.top);
    soldierBox.setSize({ soldierBounds.width, soldierBounds.height });
    soldierBox.setOutlineColor(sf::Color::Blue);
    soldierBox.setOutlineThickness(1.f);
    soldierBox.setFillColor(sf::Color::Transparent);

    FloatRect soldierLegHitbox = soldier.getLegHitbox();
    RectangleShape legHitbox;
    legHitbox.setPosition(soldierLegHitbox.left, soldierLegHitbox.top);
    legHitbox.setSize({ soldierLegHitbox.width, soldierLegHitbox.height });
    legHitbox.setOutlineColor(sf::Color::Yellow);
    legHitbox.setOutlineThickness(1.f);
    legHitbox.setFillColor(sf::Color::Transparent);

    //Clock npcTalkTime;
    window.setVisible(true);
    // Main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            // Close window on request
            if (event.type == Event::Closed)
                window.close();
            // If F was pressed and the player is within the range of the npc, the npc should be able to talk
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::F && (npc.getCollisionBox().intersects(soldier.getCollisionBox()) || npc.getCollisionBox().intersects(soldier.getLegHitbox()))) {
                npcShouldTalk = true;
                soldier.shouldMove(false); // The player shouldn't be able to move while talking
            }
            //If Enter was pressed the npc shouldn't be able to talk anymore
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                npcShouldTalk = false;
                soldier.shouldMove(true);
            }
            if (event.type == sf::Event::Resized) {
                //LetterBoxView(window, view, event.size.width, event.size.height);
                view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
                grassBlocks.regenerateTiles("assets/grass2.png", 0, 0, event.size.height / TILE_SIZE + 1, event.size.width / TILE_SIZE + 1);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11) {
                toggleFullscreen(window, fullscreen, view);
            }
        }

        if (moveTime.getElapsedTime().asSeconds() > moveDelay) {
            //If key is pressed check which one is it
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                soldier.moveLeft(rock, waterBlocks);
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                soldier.moveRight(rock, waterBlocks, window);
            }
            if (Keyboard::isKeyPressed(Keyboard::S)) {
                soldier.moveDown(rock, waterBlocks, window);
            }
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                soldier.moveUp(rock, waterBlocks);
            }
            moveTime.restart();
        }

        // Clear the screen with black
        window.clear(Color::Black); // Clear old frame
        window.draw(grassBlocks);
        window.draw(waterBlocks);
        rock.draw(window);
        //window.draw(debugBox);
        soldier.draw(window);
        npc.draw(window);
        //window.draw(soldierBox);
        //window.draw(legHitbox);
        if (npcShouldTalk) {
            npc.talk(window, soldier);
        }
        window.display(); // Tell the app that the window is done drawing
    }

}

