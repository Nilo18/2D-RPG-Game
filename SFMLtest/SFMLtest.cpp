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
#include "structuress.h"
using namespace sf;
using namespace std;
const float moveDelay = 0.03f;
const float npcTalkDuration = 5.f;
bool npcShouldTalk = false;
VideoMode desktop = VideoMode::getDesktopMode();
Tileset grassBlocks("assets/grass2.png", 0, 0, desktop.height / TILE_SIZE + 1, desktop.width / TILE_SIZE + 1); // Globally declared grass block
Tileset waterBlocks("assets/water7.png", 0, 0, 3, 5);
 
static void toggleFullscreen(sf::RenderWindow& window, bool& fullscreen, sf::View& view) {
    fullscreen = !fullscreen; // This line makes sure that both setting full screen and exiting it are run on the same button (F11)
    sf::Vector2u windowedSize(990, 880);  // Basic size of window

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

    // Regenerate tiles on resolution change
    grassBlocks.regenerateTiles("assets/grass2.png", 0, 0, currentSize.y / TILE_SIZE + 1, currentSize.x / TILE_SIZE + 1);
    waterBlocks.regenerateTiles("assets/water7.png", 0, 0, 3, 5);
}

int main() {
    // Create a window with title and size
    RenderWindow window(desktop, "SFML Test Window", Style::Fullscreen);
    //window.setVisible(false);
    View view;
    view.setSize(static_cast<float>(desktop.width), static_cast<float>(desktop.height));
    view.setCenter(desktop.width / 2.f, desktop.height / 2.f);
    window.setView(view);

    bool fullscreen = true;
     
    // To determine how many blocks we have to draw we divide the width and height by the tile size and add 1
    // First value of Grass/Water Group takes the texture, second and third starting x and y positions and fourth and fifth the area on which they should span
    Rock rock("assets/rock2.png", 500, 500);
    House house("assets/HOUSe.png", 120, 600);
    FloatRect houseHitbox = house.getCollisionBox();
    RectangleShape houseBox;
    houseBox.setPosition(houseHitbox.left, houseHitbox.top);
    houseBox.setSize({ houseHitbox.width, houseHitbox.height });
    houseBox.setOutlineColor(Color::Blue);
    houseBox.setFillColor(Color::Transparent);
    houseBox.setOutlineThickness(1.f);
    Human soldier("assets/avtandila.png", 100.f, 250.f);
    NPC npc("assets/avtandila.png", 500.f, 0.0f);
    NPC npc1("assets/avtandila.png", 700.f, 0.0f);
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
                // Regenerate tiles on resolution change
                grassBlocks.regenerateTiles("assets/grass2.png", 0, 0, event.size.height / TILE_SIZE + 1, event.size.width / TILE_SIZE + 1);
                waterBlocks.regenerateTiles("assets/water7.png", 0, 0, 3, 5);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11) {
                toggleFullscreen(window, fullscreen, view);
            }
        }

        if (moveTime.getElapsedTime().asSeconds() > moveDelay) {
            //If key is pressed check which one is it
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                soldier.moveLeft(rock, waterBlocks, house);
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                soldier.moveRight(rock, waterBlocks, window, house);
            }
            if (Keyboard::isKeyPressed(Keyboard::S)) {
                soldier.moveDown(rock, waterBlocks, window, house);
            }
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                soldier.moveUp(rock, waterBlocks, house);
            }
            moveTime.restart();
        }

        // Clear the screen with black
        window.clear(Color::Black); // Clear old frame
        window.draw(grassBlocks);
        window.draw(waterBlocks);
        rock.draw(window);
        window.draw(house);
        //window.draw(debugBox);
        soldier.draw(window);
        window.draw(houseBox);
        npc.draw(window);
        //window.draw(soldierBox);
        //window.draw(legHitbox);
        if (npcShouldTalk) {
            npc.talk(window, soldier);
        }
        window.display(); // Tell the app that the window is done drawing
    }

}

