#include "characters.h"

// Function for checking rock's collision (circular hitbox) with player (rectangle hitbox)
bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect) {
    float closestX = clamp(cx, rect.left, rect.left + rect.width);
    float closestY = clamp(cy, rect.top, rect.top + rect.height);

    float distanceX = cx - closestX;
    float distanceY = cy - closestY;

    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}

// Functions related to Human
Human::Human(const string& texturePath, int startX, int startY) {
    this->startX = startX;
    this->startY = startY;
    if (!infTexture.loadFromFile(texturePath)) {
        throw runtime_error("Failed to load texture");
    }
    infSprite.setTexture(infTexture);
    infSprite.setPosition(this->startX, this->startY);
}

FloatRect Human::getCollisionBox() {
    FloatRect boundingBox = infSprite.getGlobalBounds();
    boundingBox.top += 20;
    boundingBox.left += 22;
    boundingBox.width -= 66;
    boundingBox.height -= 65;
    return boundingBox;
}

FloatRect Human::getLegHitbox() {
    FloatRect nextBounds = getCollisionBox();

    FloatRect legsBox = {
        nextBounds.left + 14.f,  // Offset from left
        nextBounds.top + 65.f,  // Offset from top
        37.f,                    // Fixed width
        35.f                     // Fixed height
    };

    return legsBox;
}

bool Human::infantryIsColliding(int offsetX, int offsetY, Rock& rock, WaterGroup& waterBlocks, Water* water) {
    FloatRect nextBounds = getCollisionBox();
    // Look ahead of the current position by the given offsets
    nextBounds.left += offsetX;
    nextBounds.top += offsetY;

    FloatRect legsBox = getLegHitbox();
    legsBox.left += offsetX;
    legsBox.top += offsetY;

    CircleHitbox rockHitbox = rock.getCollisionBoxData();

    if (circleIntersectsRect(rockHitbox.centerX, rockHitbox.centerY, rockHitbox.radius, nextBounds) ||
        circleIntersectsRect(rockHitbox.centerX, rockHitbox.centerY, rockHitbox.radius, legsBox)) {
        return true;
    }
    else if (water != nullptr && nextBounds.intersects(water->getSprite().getGlobalBounds())) {
        return true;
    }
    else if (waterBlocks.getWaterTiles().size() != 0) {
        for (auto* waterBlock : waterBlocks.getWaterTiles()) {
            if (nextBounds.intersects(waterBlock->getSprite().getGlobalBounds())) {
                return true;
            }
        }
    }
    return false;
}

void Human::moveLeft(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startX > 0 && !infantryIsColliding(-10, 0, rock, waterBlocks, water)) {
        startX -= 10;
        infSprite.setPosition(startX, startY);
    }
}

void Human::moveRight(Rock& rock, WaterGroup& waterBlocks, RenderWindow& window, Water* water) {
    float rightEdge = window.getSize().x;
    float soldierRight = infSprite.getPosition().x + infSprite.getGlobalBounds().width;
    if (soldierRight <= rightEdge && !infantryIsColliding(10, 0, rock, waterBlocks, water)) {
        startX += 10;
        infSprite.setPosition(startX, startY);
    }
}

void Human::moveDown(Rock& rock, WaterGroup& waterBlocks, RenderWindow& window, Water* water) {
    float bottomEdge = window.getSize().y;
    float soldierBottom = infSprite.getPosition().y + infSprite.getGlobalBounds().height;
    if (soldierBottom <= bottomEdge && !infantryIsColliding(0, 10, rock, waterBlocks, water)) {
        startY += 10;
        infSprite.setPosition(startX, startY);
    }
}

void Human::moveUp(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startY > 0 && !infantryIsColliding(0, -10, rock, waterBlocks, water)) {
        startY -= 10;
        infSprite.setPosition(startX, startY);
    }
}

void Human::draw(RenderWindow& window) {
    window.draw(infSprite);
}

void Human::setScale(float scaleX, float scaleY) {
    infSprite.setScale(scaleX, scaleY);
}

// Functions related to NPC
NPC::NPC(const string& texturePath, int startX, int startY) {
    if (!npcTexture.loadFromFile(texturePath)) {
        throw runtime_error("Couldn't load NPC texture.");
    }
    npcSprite.setTexture(npcTexture);
    this->startX = startX;
    this->startY = startY;
    npcSprite.setPosition(this->startX, this->startY);
}