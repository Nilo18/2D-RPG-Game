#pragma once
//#ifndef NATURE_H
//#define NATURE_H
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

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
    int startX;
    int startY;
public:
    NatureObject(const string& texturePath, int startX, int startY);
    const Sprite& getSprite() const;
    virtual FloatRect getCollisionBox(); // We don't return by a const reference here because we're returning a temporary variable, created in the body
    virtual CircleHitbox getCollisionBoxData();
    virtual void draw(RenderWindow& window);
};

class Grass : public NatureObject {
public:
    Grass(const string& texturePath, int startX, int startY);
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

class Water : public NatureObject {
public:
    Water(const string& texturePath, int startX, int startY);
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

class Rock : public NatureObject {
public:
    Rock(const string& texturePath, float startX, float startY);
    FloatRect getCollisionBox() override; // We don't return by a const reference here because we're returning a temporary variable, created in the body
    CircleHitbox getCollisionBoxData() override;
};

extern const int TILE_SIZE;
extern const int TILES_X;  // = 15
extern const int TILES_Y;   // = 12

//#endif