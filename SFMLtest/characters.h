#pragma once
#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <SFML/Graphics.hpp>
#include "nature.h"
#include "structuress.h"
#include <iostream>
#include "Game_GUI.h"
#include "utils.h"
#include <array>
using namespace sf;
using namespace std;

// Base class for all characters/entities
class Entity {
protected:
    Sprite sprite;
    Texture texture;
    float startX;
    float startY;
public:
    Entity(const string& texturePath, float startX, float startY);
    virtual FloatRect getCollisionBox(); 
    void setScale(float scaleX, float scaleY);
    virtual void draw(RenderWindow& window);
};

class Human : public Entity {
private:
    bool shouldBeAbleToMove = true;
public:
    Human(const string& texturePath, float startX, float startY);
    void moveLeft(Rock& rock, Tileset& waterBlocks, House& house, Tile* water = nullptr);
    void moveRight(Rock& rock, Tileset& waterBlocks, RenderWindow& window, House& house, Tile* water = nullptr);
    void moveDown(Rock& rock, Tileset& waterBlocks, RenderWindow& window, House& house, Tile* water = nullptr);
    void moveUp(Rock& rock, Tileset& waterBlocks, House& house, Tile* water = nullptr);
    //void draw(RenderWindow& window) override;
    // Default values must be the last parameters, or all of the following parameters after it must also be default
    bool infantryIsColliding(int offsetX, int offsetY, Rock& rock, Tileset& waterBlocks, House& house, Tile* water = nullptr); // Take all collidable objects as parameters to check for each 
    bool shouldAppearBehind(House& house);
    FloatRect getCollisionBox() override; // We don't return by a const reference here because we're returning a temporary variable, created in the body
    FloatRect getLegHitbox();
    void shouldMove(bool val);
};

bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect);

class NPC : public Entity {
private:
//    Font font;
public:
    NPC(const string& texturePath, float startX, float startY);
    void talk(RenderWindow& window, Human& player);
};


#endif