#pragma once
#ifndef NATURE_H
#define NATURE_H
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class Grass {
private:
    Sprite grassSprite;
    Texture grassTexture;
    int startX;
    int startY;
public:
    Grass(const string& texturePath, int startX, int startY);
    const Sprite& getSprite(); // Return by a const reference to avoid modification via the getter
};

class GrassGroup : public Drawable {
private:
    vector<Grass*> grassTiles;
public:
    // We're using floats because setPosition takes floats plus it allows for smoother movement if ever needed
    GrassGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
    ~GrassGroup();
    const vector <Grass*>& getGrassTiles() const;
    virtual void draw(RenderTarget& target, RenderStates states) const override; // This method will be called internally when drawing the object
};

class Water {
private:
    Sprite waterSprite;
    Texture waterTexture;
    int startX;
    int startY;
public:
    Water(const string& texturePath, int startX, int startY);
    const Sprite& getSprite() const;
};

class WaterGroup : public Drawable {
private:
    vector<Water*> waterTiles;
public:
    WaterGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
    ~WaterGroup();
    const vector<Water*>& getWaterTiles() const;
    virtual void draw(RenderTarget& target, RenderStates states) const override;
    const Sprite& getSprite() const;
};

struct CircleHitbox {
    float centerX;
    float centerY;
    float radius;
};

class Rock {
private:
    Sprite rockSprite;
    Texture rockTexture;
    int startX;
    int startY;
public:
    Rock(const string& texturePath, float startX, float startY);
    void draw(RenderWindow& window);
    const Sprite& getSprite() const;
    FloatRect getCollisionBox(); // We don't return by a const reference here because we're returning a temporary variable, created in the body
    CircleHitbox getCollisionBoxData();
};


extern const int TILE_SIZE;
extern const int TILES_X;  // = 15
extern const int TILES_Y;   // = 12

#endif