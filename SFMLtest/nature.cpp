#include "nature.h"
const int TILE_SIZE = 64;
const int TILES_X = 1000 / TILE_SIZE;
const int TILES_Y = 800 / TILE_SIZE;

// NatureObject base class methods
NatureObject::NatureObject(const string& texturePath, int startX, int startY) {
    if (!texture.loadFromFile(texturePath)) {
        throw runtime_error("Couldn't load a nature object.");
    }
    sprite.setTexture(texture);
    this->startX = startX;
    this->startY = startY;
    sprite.setPosition(this->startX, this->startY);
}

void NatureObject::draw(RenderWindow& window) {
    window.draw(sprite);
}

const Sprite& NatureObject::getSprite() const { return sprite; }

FloatRect NatureObject::getCollisionBox() {
    return sprite.getGlobalBounds();
}

CircleHitbox NatureObject::getCollisionBoxData() {
    return { 0, 0, 0 };
}

// Grass methods (one block)
Grass::Grass(const string& texturePath, int startX, int startY) : NatureObject(texturePath, startX, startY) {}

// GrassGroup methods
void GrassGroup::draw(RenderTarget& target, RenderStates states) const {
    for (auto* tile : grassTiles) {
        target.draw(tile->getSprite());
    }
}

GrassGroup::GrassGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
    for (int row = 0; row < rowsToSpan; row++) {
        for (int col = 0; col < colsToSpan; col++) {
            float x = startX + col * TILE_SIZE;
            float y = startY + row * TILE_SIZE;
            grassTiles.push_back(new Grass(texturePath, x, y));
        }
    }
}

GrassGroup::~GrassGroup() {
    for (auto* tile : grassTiles) {
        delete tile;
    }
    grassTiles.clear();
}

const vector <Grass*>& GrassGroup::getGrassTiles() const { return grassTiles; }

// Water methods (1 block of water)
Water::Water(const string& texturePath, int startX, int startY) : NatureObject(texturePath, startX, startY) {}

//const Sprite& Water::getSprite() const { return waterSprite; }

// WaterGroup methods
WaterGroup::WaterGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
    for (int row = 0; row < rowsToSpan; row++) {
        for (int col = 0; col < colsToSpan; col++) {
            float x = startX + col * TILE_SIZE;
            float y = startY + row * TILE_SIZE;
            waterTiles.push_back(new Water(texturePath, x, y));
        }
    }
}

WaterGroup::~WaterGroup() {
    for (auto* tile : waterTiles) {
        delete tile;
    }
    waterTiles.clear();
}

void WaterGroup::draw(RenderTarget& target, RenderStates states) const {
    for (auto* tile : waterTiles) {
        target.draw(tile->getSprite());
    }
}

const vector<Water*>& WaterGroup::getWaterTiles() const { return waterTiles; }

// Rock methods
Rock::Rock(const string& texturePath, float startX, float startY) : NatureObject(texturePath, startX, startY) {}

//const Sprite& Rock::getSprite() const { return rockSprite; }

FloatRect Rock::getCollisionBox() {
    float diameter = sprite.getGlobalBounds().width * 0.5f;
    float radius = diameter / 2.f;

    float centerX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
    float centerY = sprite.getPosition().y + sprite.getGlobalBounds().height / 2.f;

    centerY += 1.1;

    return FloatRect(centerX - radius, centerY - radius, diameter, diameter);
}

CircleHitbox Rock::getCollisionBoxData() {
    float diameter = sprite.getGlobalBounds().width * 0.6f;
    float radius = diameter / 2.f;

    float centerX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
    float centerY = sprite.getPosition().y + sprite.getGlobalBounds().height / 2.f;

    centerY += 1.1;

    return { centerX, centerY, radius };
}

