#pragma once
#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <SFML/Graphics.hpp>
#include "nature.h"
using namespace sf;
using namespace std;

class Human {
private:
    Sprite infSprite;
    Texture infTexture;
    int startX;
    int startY;
public:
    Human(const string& texturePath, int startX, int startY);
    void moveLeft(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    void moveRight(Rock& rock, WaterGroup& waterBlocks, RenderWindow& window, Water* water = nullptr);
    void moveDown(Rock& rock, WaterGroup& waterBlocks, RenderWindow& window, Water* water = nullptr);
    void moveUp(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    void draw(RenderWindow& window);
    void setScale(float scaleX, float scaleY);
    bool infantryIsColliding(int offsetX, int offsetY, Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr); // Take all collidable objects as parameters to check for each 
    FloatRect getCollisionBox(); // We don't return by a const reference here because we're returning a temporary variable, created in the body
    FloatRect getLegHitbox();
};

bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect);

class NPC {
private:
    Sprite npcSprite;
    Texture npcTexture;
    int startX;
    int startY;
public:
    NPC(const string& texturePath, int startX, int startY);
    void talk();
};


#endif