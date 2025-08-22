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
    return sprite.getGlobalBounds(); // Returns sprite's global bounds on default
}

CircleHitbox NatureObject::getCollisionBoxData() {
    return { 0, 0, 0 }; // Returns 0s on default
}

// Tile methods
Tile::Tile(Texture& tex, float startX, float startY) {
    sprite.setTexture(tex);
    sprite.setPosition(startX, startY);
}

Tile::Tile() = default;

const Sprite& Tile::getSprite() const { return sprite; }

// Tileset methods
Tileset::Tileset(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
	// Load the texture here to avoid dangling texture pointers in tiles, this way the texture is loaded in one place and passed by reference to each tile
    if (!texture.loadFromFile(texturePath)) throw runtime_error("Couldn't load texture");
    for (int row = 0; row < rowsToSpan; row++) {
        for (int col = 0; col < colsToSpan; col++) {
            float x = startX + col * TILE_SIZE;
            float y = startY + row * TILE_SIZE;
            tiles.push_back(make_unique<Tile>(texture, x, y));
        }
    }
}

void Tileset::draw(RenderTarget& target, RenderStates states) const {
    for (const auto& tile : tiles) {
        target.draw(tile->getSprite(), states);
    }
}
const vector<unique_ptr<Tile>>& Tileset::getTiles() const { return tiles; }

// Method for regenerating the tiles after the rowsToSpan and colsToSpan (screen sizes) change
void Tileset::regenerateTiles(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan) {
	// Make sure to load the texture here to avoid dangling texture reference in tiles
    // When we regenerate the tiles, the old one's which loaded the texture were destroyed and if we don't load the new texture 
	// to adjust to the changed resolution, dangling pointers will occur
    if (!texture.loadFromFile(texturePath)) throw runtime_error("Couldn't load texture");
    // Clear the previous tiles first
    tiles.clear();
    // Now regenerate
    for (int row = 0; row < rowsToSpan; row++) {
        for (int col = 0; col < colsToSpan; col++) {
            float x = startX + col * TILE_SIZE;
            float y = startY + row * TILE_SIZE;
            tiles.push_back(make_unique<Tile>(this->texture, x, y));
        }
    }
}

// Rock methods
Rock::Rock(const string& texturePath, float startX, float startY) : NatureObject(texturePath, startX, startY) {}

CircleHitbox Rock::getCollisionBoxData() {
    float diameter = sprite.getGlobalBounds().width * 0.6f;
    float radius = diameter / 2.f;

    float centerX = sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f;
    float centerY = sprite.getPosition().y + sprite.getGlobalBounds().height / 2.f;

    centerY += 1.1f;

    return { centerX, centerY, radius };
}

