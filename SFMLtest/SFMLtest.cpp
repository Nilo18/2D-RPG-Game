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

int main() {
    // Create a window with title and size
    //std::cout << "Working directory is: " << filesystem::current_path() << std::endl;
    RenderWindow window(sf::VideoMode(992, 800), "SFML Test Window");
     
    // To determine how many blocks we have to draw we divide the width and height by the tile size and add 1
    // First value of Grass/Water Group takes the texture, second and third starting x and y positions and fourth and fifth the area on which they should span
    GrassGroup grassBlocks("assets/grass2.png", 0, 0, window.getSize().y / TILE_SIZE + 1, window.getSize().x / TILE_SIZE + 1);
    WaterGroup waterBlocks("assets/water.png", 0, 0, 3, 5);
    Rock rock("assets/rock2.png", 500, 500);
    Human soldier("assets/infantry2.png", 100, 250);
    soldier.setScale(2.f, 2.f);  // doubles width and height

    // Main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            // Close window on request
            switch (event.type) {
                case Event::Closed:
                    window.close(); break;
                     //If key is pressed check which one is it
                case Event::KeyPressed:
                    switch (event.key.code) {
                    case Keyboard::A:
                        soldier.moveLeft(rock, waterBlocks); break;
                    case Keyboard::D:
                        soldier.moveRight(rock, waterBlocks); break;
                    case Keyboard::S:
                        soldier.moveDown(rock, waterBlocks); break;
                    case Keyboard::W:   
                        soldier.moveUp(rock, waterBlocks); break;
                    }
            }
                
        }
        // Clear the screen with black
        window.clear(Color(105, 255, 255, 255)); // Clear old frame
        window.draw(grassBlocks);
        window.draw(waterBlocks);
        rock.draw(window);
        soldier.draw(window);
        window.display(); // Tell the app that the window is done drawing
    }

}

