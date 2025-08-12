#include "nature.h"

// NatureObject base class methods
NatureObject::NatureObject(const string& texturePath, float startX, float startY) {
    if (!texture.loadFromFile(texturePath)) {
        cerr<< "Couldn't load a nature object.";
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

// Tile methods
Tile::Tile(const string& texturePath, float startX, float startY) {
    if (!texture.loadFromFile(texturePath)) {
        cerr << "Couldn't load texture.";
    }
    sprite.setTexture(texture);
    this->startX = startX;
    this->startY = startY;
    sprite.setPosition(startX, startY);
}

Tile::Tile() = default;

const Sprite& Tile::getSprite() const { return sprite; }

// Tileset methods
    // Methods have to be defined inside the class because templates require compile time definitions
Tileset::Tileset(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
    for (int row = 0; row < rowsToSpan; row++) {
        for (int col = 0; col < colsToSpan; col++) {
            float x = startX + col * TILE_SIZE;
            float y = startY + row * TILE_SIZE;
            tiles.push_back(new Tile(texturePath, x, y));
        }
    }
}

Tileset::~Tileset() {
    for (auto* tile : tiles) {
        delete tile;
    }
}
void Tileset::draw(RenderTarget& target, RenderStates states) const {
    for (auto* tile : tiles) {
        target.draw(tile->getSprite(), states);
    }
}
const vector<Tile*>& Tileset::getTiles() const { return tiles; }

// Method for regenerating the tiles after the rowsToSpan and colsToSpan (screen sizes) change
void Tileset::regenerateTiles(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
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
            tiles.push_back(new Tile(texturePath, x, y));
        }
    }
}

// Rock methods
Rock::Rock(const string& texturePath, float startX, float startY) : NatureObject(texturePath, startX, startY) {}

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

