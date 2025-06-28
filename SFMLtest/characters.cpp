#include "characters.h"

// Function for checking rock's collision (circular hitbox) with player (rectangle hitbox)
bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect) {
    float closestX = clamp(cx, rect.left, rect.left + rect.width);
    float closestY = clamp(cy, rect.top, rect.top + rect.height);

    float distanceX = cx - closestX;
    float distanceY = cy - closestY;

    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}

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
    if (startX > 0 && !infantryIsColliding(-5, 0, rock, waterBlocks, water)) {
        startX -= 5;
        infSprite.setPosition(startX, startY);
    }
}

void Human::moveRight(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startX <= 1000 && !infantryIsColliding(5, 0, rock, waterBlocks, water)) {
        startX += 5;
        infSprite.setPosition(startX, startY);
    }
}

void Human::moveDown(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startY <= 800 && !infantryIsColliding(0, 5, rock, waterBlocks, water)) {
        startY += 5;
        infSprite.setPosition(startX, startY);
    }
}

void Human::moveUp(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startY > 0 && !infantryIsColliding(0, -5, rock, waterBlocks, water)) {
        startY -= 5;
        infSprite.setPosition(startX, startY);
    }
}

void Human::draw(RenderWindow& window) {
    window.draw(infSprite);
}

void Human::setScale(float scaleX, float scaleY) {
    infSprite.setScale(scaleX, scaleY);
}