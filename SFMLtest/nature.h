#pragma once
//#ifndef NATURE_H
//#define NATURE_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.h"
#include "CollisionManager.h"
#include <memory>
using namespace sf;
using namespace std;

// constexpr makes the compiler determine the values at compile time (before the CPU executes the commands)
constexpr int TILE_SIZE = 64;
constexpr int TILES_X = 1000 / TILE_SIZE;
constexpr int TILES_Y = 800 / TILE_SIZE;

// Base class for every natural object
class NatureObject : public Drawable {
protected:
    Sprite sprite;
    Texture texture;
    float startX = 0.f;
    float startY = 0.f;
public:
    NatureObject(const string& texturePath, float startX, float startY);
    const Sprite& getSprite() const;
    // This is normal rectangular hitbox for normal objects
    //virtual FloatRect getCollisionBox(); // We don't return by a const reference here because we're returning a temporary variable, created in the body
    // This is circle hitbox for round objects
    virtual CircleHitbox getCollisionBoxData();
    //virtual void draw(RenderWindow& window);
	virtual void draw(RenderTarget& target, RenderStates states) const override;
};

// Base class for every type of tiles
class Tile {
private:
    Sprite sprite;
    //Texture texture;
    float startX = 0.f;
    float startY = 0.f;
public:
    Tile(Texture& tex, float startX, float startY);
    Tile(); // For arrays
    const Sprite& getSprite() const;
};

// Base class for every type of tileset
class Tileset : public Drawable {
protected:
    Texture texture; // Texture should live as long as the tileset
    vector<unique_ptr<Tile>> tiles;
public:
    Tileset(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
    virtual void draw(RenderTarget& target, RenderStates states) const override;
    const vector<unique_ptr<Tile>>& getTiles() const;
    // Method for regenerating the tiles after the rowsToSpan and colsToSpan (screen sizes) change
    void regenerateTiles(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
};

// We need water to be collidable, that's why we separate it from the Tileset class
class WaterGroup : public Tileset, public Collidable {
private:
	RectangleHitbox waterHitbox;
public:
	WaterGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
    void setHitboxOffset(float left, float top, float width, float height) override;
	const Hitbox* getCollisionBox(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) override;
};

// Declare public explicitly to avoid private inheritance by default
class Rock : public NatureObject, public Collidable {
private:
    CircleHitbox rockHitbox;
public:
    Rock(const string& texturePath, float startX, float startY);
    void setHitboxOffset(float left, float top, float width, float height) override;
    CircleHitbox getCollisionBoxData() override; // We don't return by a const reference here because we're returning a temporary variable, created in the body
    const Hitbox* getCollisionBox(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) override;
};

//#endif