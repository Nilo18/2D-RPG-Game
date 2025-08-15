#pragma once
//#ifndef NATURE_H
//#define NATURE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.h"
using namespace sf;
using namespace std;

// constexpr makes the compiler determine the values at compile time (before the CPU executes the commands)
constexpr int TILE_SIZE = 64;
constexpr int TILES_X = 1000 / TILE_SIZE;
constexpr int TILES_Y = 800 / TILE_SIZE;

struct CircleHitbox {
    // Initialize the values by 0 on default
    float centerX = 0.f;
    float centerY = 0.f;
    float radius = 0.f;
};

// Base class for every natural object
class NatureObject {
protected:
    Sprite sprite;
    Texture texture;
    float startX = 0.f;
    float startY = 0.f;
public:
    NatureObject(const string& texturePath, float startX, float startY);
    const Sprite& getSprite() const;
    // This is normal rectangular hitbox for normal objects
    virtual FloatRect getCollisionBox(); // We don't return by a const reference here because we're returning a temporary variable, created in the body
    // This is circle hitbox for round objects
    virtual CircleHitbox getCollisionBoxData();
    virtual void draw(RenderWindow& window);
};

// Base class for every type of tiles
class Tile {
private:
    Sprite sprite;
    Texture texture;
    float startX = 0.f;
    float startY = 0.f;
public:
    Tile(const string& texturePath, float startX, float startY);
    Tile(); // For arrays
    const Sprite& getSprite() const;
};


// Base class for every type of tileset
class Tileset : public Drawable {
private:
    vector<Tile*> tiles;
public:
    Tileset(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
    ~Tileset();
    virtual void draw(RenderTarget& target, RenderStates states) const override;
    const vector<Tile*>& getTiles() const;
    // Method for regenerating the tiles after the rowsToSpan and colsToSpan (screen sizes) change
    void regenerateTiles(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
};

class Rock : public NatureObject {
public:
    Rock(const string& texturePath, float startX, float startY);
    FloatRect getCollisionBox() override; // We don't return by a const reference here because we're returning a temporary variable, created in the body
    CircleHitbox getCollisionBoxData() override;
};

//#endif