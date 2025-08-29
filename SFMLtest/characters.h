#pragma once
#ifndef CHARACTERS_H
#define CHARACTERS_H
#include <SFML/Graphics.hpp>
#include "nature.h"
#include "structuress.h"
#include <iostream>
#include "Game_GUI.h"
#include "utils.h"
#include "CollisionManager.h"
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
    //virtual FloatRect getCollisionBox(); 
    void setScale(float scaleX, float scaleY);
    virtual void draw(RenderWindow& window);
};

class Human : public Entity, public Collidable {
private:
    RectangleHitbox collisionBox;
	RectangleHitbox legHitbox; // Smaller hitbox for legs to allow better movement around obstacles
    bool shouldBeAbleToMove = true;
public:
    Human(const string& texturePath, float startX, float startY);
    void moveLeft();
    void moveRight(RenderWindow& window);
    void moveDown(RenderWindow& window);
    void moveUp();
    //void draw(RenderWindow& window) override;
    // Default values must be the last parameters, or all of the following parameters after it must also be default
    //bool infantryIsColliding(int offsetX, int offsetY, Rock& rock, Tileset& waterBlocks, House& house, Tile* water = nullptr); // Take all collidable objects as parameters to check for each 
    bool shouldAppearBehind(const std::shared_ptr<House>& house);
    const Hitbox* getCollisionBox(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) override; // We don't return by a const reference here because we're returning a temporary variable, created in the body
    const Hitbox* getLegHitbox(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f);
    void shouldMove(bool val);
    void setHitboxOffset(float left, float top, float width, float height) override;
};

class NPC : public Entity, public Collidable {
private:
    RectangleHitbox npcHitbox;
public:
    NPC(const string& texturePath, float startX, float startY);
    void talk(RenderWindow& window);
    void setHitboxOffset(float left, float top, float width, float height) override;
    const Hitbox* getCollisionBox(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) override;
};


#endif