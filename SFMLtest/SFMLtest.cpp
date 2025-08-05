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
 
void LetterBoxView(sf::RenderWindow& window, sf::View& view, int wWidth, int wHeight) {
    float windowRatio = (float)wWidth / wHeight;
    float viewRatio = view.getSize().x / view.getSize().y;
    float sizeX = 1.f, sizeY = 1.f;
    float posX = 0.f, posY = 0.f;

    if (windowRatio > viewRatio)
    {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX)/2.f;
    }
    else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY)/2.f;
    }
    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    window.setView(view);
}
void toggleFullscreen(sf::RenderWindow& window, bool& fullscreen, sf::View& view) {
    fullscreen = !fullscreen;
    sf::Vector2u basesize(990, 880);  // Basic size of window

    window.close();
    if (fullscreen)
        window.create(sf::VideoMode::getDesktopMode(), "SFML", sf::Style::Fullscreen);
    else
        window.create(sf::VideoMode(basesize.x, basesize.y), "SFML", sf::Style::Default);
    
    view.setSize((float)basesize.x, (float)basesize.y);
    view.setCenter(basesize.x / 2.f, basesize.y / 2.f);
    window.setView(view);

    LetterBoxView(window, view, window.getSize().x, window.getSize().y);
}
int main() {
    // Create a window with title and size
    //std::cout << "Working directory is: " << filesystem::current_path() << std::endl; 
    sf::Vector2u basesize = { 990,800 };
    sf::RenderWindow window(sf::VideoMode(basesize.x, basesize.y), "SFML Test Window", sf::Style::Default);
    window.setVisible(false);
    sf::View view(sf::FloatRect(0, 0, basesize.x, basesize.y));
    view.setCenter(basesize.x / 2.f, basesize.y / 2.f);
    window.setView(view);

    bool fullscreen = false;
    
    
     
    // To determine how many blocks we have to draw we divide the width and height by the tile size and add 1
    // First value of Grass/Water Group takes the texture, second and third starting x and y positions and fourth and fifth the area on which they should span
    GrassGroup grassBlocks("assets/grass2.png", 0, 0, window.getSize().y / TILE_SIZE + 1, window.getSize().x / TILE_SIZE + 1);
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
            if (event.type == sf::Event::Resized)
                LetterBoxView(window, view, event.size.width, event.size.height);
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11)
                toggleFullscreen(window, fullscreen, view);
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

