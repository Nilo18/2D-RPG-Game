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
#include "CollisionManager.h"
using namespace sf;
using namespace std;
const float moveDelay = 0.03f;
const float npcTalkDuration = 5.f;
bool npcShouldTalk = false;
VideoMode desktop = VideoMode::getDesktopMode();
Tileset grassBlocks("assets/grass2.png", 0, 0, desktop.height / TILE_SIZE + 1, desktop.width / TILE_SIZE + 1); // Globally declared grass block
shared_ptr<WaterGroup> waterBlocks = make_shared<WaterGroup>("assets/water7.png", 0, 0, 3, 5);

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
    waterBlocks->regenerateTiles("assets/water7.png", 0, 0, 3, 5);
}

int main() {
    // Create a window with title and size
    RenderWindow window(desktop, "SFML Test Window", Style::Fullscreen);
    CollisionManager collisionManager;
    //waterBlocks->setHitboxOffset(0.f, 0.f, 0.f, 0.f);
    collisionManager.registerObject(static_pointer_cast<Collidable>(waterBlocks));
    window.setFramerateLimit(60); // Set frame rate limit to 60 fps to avoid using 100% of CPU
    //window.setVisible(false);
    View view;
    view.setSize(static_cast<float>(desktop.width), static_cast<float>(desktop.height));
    view.setCenter(desktop.width / 2.f, desktop.height / 2.f);
    window.setView(view);

    bool fullscreen = true;

    // To determine how many blocks we have to draw we divide the width and height by the tile size and add 1
    // First value of Grass/Water Group takes the texture, second and third starting x and y positions and fourth and fifth the area on which they should span
    shared_ptr<Rock> rock = make_shared<Rock>("assets/rock2.png", 500, 500);
	collisionManager.registerObject(static_pointer_cast<Collidable>(rock)); // cast rock to Collidable and register it
    shared_ptr<House> house = make_shared<House>("assets/HOUSe.png", 120, 600);
	house->setHitboxOffset(118.f, 153.f, 229.f, 311.f); // Predifine the hitbox offset for the house so don't have to pass the arguments every time we create a house object
    collisionManager.registerObject(static_pointer_cast<Collidable>(house));
    FloatRect houseHitbox = house->getCollisionBox()->getRect();
    RectangleShape houseBox = utilities::createRectangleDebugBox(houseHitbox, Color::Blue, Color::Transparent, 1.f);
    TriangleHitbox roofHitbox = house->getRoofCollisionBox();
    ConvexShape roofDebugBox = utilities::createTriangleDebugBox(roofHitbox, Color::Green, Color::Transparent, 2.f);

    shared_ptr<Human> soldier = make_shared<Human>("assets/avtandila.png", 100.f, 250.f);
    soldier->setHitboxOffset(20.f, 35.f, 66.f, 65.f);
	collisionManager.registerObject(static_pointer_cast<Collidable>(soldier)); // cast soldier to Collidable and register it
    shared_ptr<NPC> npc = make_shared<NPC>("assets/avtandila.png", 500.f, 0.0f);
    collisionManager.registerObject(static_pointer_cast<Collidable>(npc)); // cast npc to Collidable and register it

    FloatRect npcBox = npc->getCollisionBox()->getRect();
    npc->setScale(2.f, 2.f);
    soldier->setScale(2.f, 2.f);  // doubles width and height

    Clock moveTime;
    FloatRect bounds = rock->getCollisionBox()->getRect();

    CircleHitbox hitbox = rock->getCollisionBoxData();

    CircleShape debugBox = utilities::createCircleDebugBox(hitbox, Color::Red, Color::Transparent, 1.f);

    FloatRect soldierBounds = soldier->getCollisionBox()->getRect();  // Or next position with offset
    sf::RectangleShape soldierBox = utilities::createRectangleDebugBox(soldierBounds, Color::Blue, Color::Transparent, 1.f);

    FloatRect soldierLegHitbox = soldier->getLegHitbox()->getRect();
    RectangleShape legHitbox = utilities::createRectangleDebugBox(soldierLegHitbox, Color::Yellow, Color::Transparent, 1.f);

    //Clock npcTalkTime;
    //std::cout << "npc ptr: " << npc.get() << std::endl;
    window.setVisible(true);
    // Main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            // Close window on request
            if (event.type == Event::Closed)
                window.close();
            // If F was pressed and the player is within the range of the npc, the npc should be able to talk
            // Check for the sides from which the player is approaching 
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::F && (collisionManager.checkCollision(soldier, 10, 0) || 
                collisionManager.checkCollision(soldier, -10, 0)
                || collisionManager.checkCollision(soldier, 0, 10)
                || collisionManager.checkCollision(soldier, 0, -10))) {
                npcShouldTalk = true;
                soldier->shouldMove(false); // The player shouldn't be able to move while talking
            }
            //If Enter was pressed the npc shouldn't be able to talk anymore
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                npcShouldTalk = false;
                soldier->shouldMove(true);
            }
            if (event.type == sf::Event::Resized) {
                //LetterBoxView(window, view, event.size.width, event.size.height);
                view.setSize(static_cast<float>(event.size.width), static_cast<float>(event.size.height));
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
                // Regenerate tiles on resolution change
                grassBlocks.regenerateTiles("assets/grass2.png", 0, 0, event.size.height / TILE_SIZE + 1, event.size.width / TILE_SIZE + 1);
                waterBlocks->regenerateTiles("assets/water7.png", 0, 0, 3, 5);
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F11) {
                toggleFullscreen(window, fullscreen, view);
            }
        }

		// Pass the offsets to get the next position of the player
        if (moveTime.getElapsedTime().asSeconds() > moveDelay) {
            //If key is pressed check which one is it
            if (Keyboard::isKeyPressed(Keyboard::A) && !collisionManager.checkCollision(soldier, -10, 0)) {
                soldier->moveLeft();
            }
            if (Keyboard::isKeyPressed(Keyboard::D) && !collisionManager.checkCollision(soldier, 10, 0)) {
				soldier->moveRight(window); // Pass window to moveRight to get the current width of the window for boundary checking
            }
            if (Keyboard::isKeyPressed(Keyboard::S) && !collisionManager.checkCollision(soldier, 0, 10)) {
                soldier->moveDown(window);
            }
            if (Keyboard::isKeyPressed(Keyboard::W) && !collisionManager.checkCollision(soldier, 0, -10)) {
                soldier->moveUp();
            }
            moveTime.restart();
        }

        // Clear the screen with black
        window.clear(Color::Black); // Clear old frame
        window.draw(grassBlocks);
        window.draw(*waterBlocks);
		window.draw(*rock);
        //window.draw(house);
        if (soldier->shouldAppearBehind(house)) {
            soldier->draw(window);
            window.draw(*house);
        }
        else {
            window.draw(*house);
            soldier->draw(window);
        }
        window.draw(debugBox);
        //soldier.draw(window);
        window.draw(houseBox);
        window.draw(roofDebugBox);
        npc->draw(window);
        window.draw(soldierBox);
        window.draw(legHitbox);
        if (npcShouldTalk) {
            npc->talk(window);
        }
        window.display(); // Tell the app that the window is done drawing
    }
}