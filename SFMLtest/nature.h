#pragma once
//#ifndef NATURE_H
//#define NATURE_H
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;

// constexpr makes the compiler determine the values at compile time (before the CPU executes the commands)
constexpr int TILE_SIZE = 64;
constexpr int TILES_X = 1000 / TILE_SIZE;
constexpr int TILES_Y = 800 / TILE_SIZE;

struct CircleHitbox {
    float centerX;
    float centerY;
    float radius;
};

// Base class for every natural object
class NatureObject {
protected:
    Sprite sprite;
    Texture texture;
    float startX;
    float startY;
public:
    NatureObject(const string& texturePath, float startX, float startY);
    const Sprite& getSprite() const;
    virtual FloatRect getCollisionBox(); // We don't return by a const reference here because we're returning a temporary variable, created in the body
    virtual CircleHitbox getCollisionBoxData();
    virtual void draw(RenderWindow& window);
};

// Base class for every type of tiles
template <typename T>
class NatureGroup : public Drawable {
protected:
    vector<T*> tiles;
public:
    // Methods have to be defined inside the class because templates require compile time definitions
    NatureGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
        for (int row = 0; row < rowsToSpan; row++) {
            for (int col = 0; col < colsToSpan; col++) {
                float x = startX + col * TILE_SIZE;
                float y = startY + row * TILE_SIZE;
                tiles.push_back(new T(texturePath, x, y));
            }
        }
    }
    NatureGroup() = default;
    virtual ~NatureGroup() {
        for (auto* tile : tiles) {
            delete tile;
        }
    }
    virtual void draw(RenderTarget& target, RenderStates states) const override {
        for (auto* tile : tiles) {
            target.draw(tile->getSprite(), states);
        }
    }
    const vector<T*>& getTiles() const { return tiles; }
    // Method for regenerating the tiles after the rowsToSpan and colsToSpan (screen sizes) change
    void regenerateTiles(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
        // Clear the previous tiles first
        for (auto* tile : tiles) {
            delete tile;
        }
        tiles.clear();
        // Now regenerate
        for (int row = 0; row < rowsToSpan; row++) {
            for (int col = 0; col < colsToSpan; col++) {
                float x = startX + col * TILE_SIZE;
                float y = startY + row * TILE_SIZE;
                tiles.push_back(new T(texturePath, x, y));
            }
        }
    }
};

class Grass : public NatureObject {
public:
    Grass(const string& texturePath, float startX, float startY);
};

class GrassGroup : public NatureGroup<Grass> {
public:
    // The following line is the same as writing
    // GrassGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) : NatureGroup(texturePath, startX, startY, rowsToSpan, colsToSpan) {}
    // It basically tells the compiler "Inherit all constructors from parent into the child without having to manually write forwarding constructors."
    using NatureGroup::NatureGroup; // So this is effectively a short-hand method, it only works if the child isn't adding/changing anything
};

class Water : public NatureObject {
public:
    Water(const string& texturePath, float startX, float startY);
};

class WaterGroup : public NatureGroup<Water> {
public:
    using NatureGroup::NatureGroup;
};

class Rock : public NatureObject {
public:
    Rock(const string& texturePath, float startX, float startY);
    FloatRect getCollisionBox() override; // We don't return by a const reference here because we're returning a temporary variable, created in the body
    CircleHitbox getCollisionBoxData() override;
};

//#endif