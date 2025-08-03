#pragma once
#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <SFML/Graphics.hpp>
#include "nature.h"
#include <iostream>
using namespace sf;
using namespace std;

// Base class for all characters/entities
class Entity {
protected:
    Sprite sprite;
    Texture texture;
    int startX;
    int startY;
public:
    Entity(const string& texturePath, int startX, int startY);
    virtual FloatRect getCollisionBox();
    void setScale(float scaleX, float scaleY);
    virtual void draw(RenderWindow& window);
};

// Private field is no longer necessary as Human is a child of Entity
class Human : public Entity {
private:
    bool shouldBeAbleToMove = true;
public:
    Human(const string& texturePath, int startX, int startY);
    void moveLeft(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    void moveRight(Rock& rock, WaterGroup& waterBlocks, RenderWindow& window, Water* water = nullptr);
    void moveDown(Rock& rock, WaterGroup& waterBlocks, RenderWindow& window, Water* water = nullptr);
    void moveUp(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    //void draw(RenderWindow& window) override;
    bool infantryIsColliding(int offsetX, int offsetY, Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr); // Take all collidable objects as parameters to check for each 
    FloatRect getCollisionBox() override; // We don't return by a const reference here because we're returning a temporary variable, created in the body
    FloatRect getLegHitbox();
    void shouldMove(bool val);
};

bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect);

class NPC : public Entity {
private:
//    Font font;
public:
    NPC(const string& texturePath, int startX, int startY);
    void talk(RenderWindow& window, Human& player);
};


#endif