#include "characters.h"

// Functions related to Entity base class
Entity::Entity(const string& texturePath, float startX, float startY) {
    if (!texture.loadFromFile(texturePath)) {
        throw runtime_error("Couldn't load texture.");
    }
    sprite.setTexture(texture);
    this->startX = startX;
    this->startY = startY;
    sprite.setPosition(this->startX, this->startY);
}

void Entity::draw(RenderWindow& window) {
    window.draw(sprite);
}

FloatRect Entity::getCollisionBox() {
    return sprite.getGlobalBounds();
}

void Entity::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
}

// Function for checking rock's collision (circular hitbox) with player (rectangle hitbox)
bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect) {
    float closestX = clamp(cx, rect.left, rect.left + rect.width);
    float closestY = clamp(cy, rect.top, rect.top + rect.height);

    float distanceX = cx - closestX;
    float distanceY = cy - closestY;

    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}

// Functions related to Human
Human::Human(const string& texturePath, float startX, float startY) : Entity(texturePath, startX, startY) {}

void Human::shouldMove(bool val) {
    shouldBeAbleToMove = val;
}
    
FloatRect Human::getCollisionBox() {
    FloatRect boundingBox = sprite.getGlobalBounds();
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

bool Human::infantryIsColliding(int offsetX, int offsetY, Rock& rock, Tileset& waterBlocks, Structure& str, Tile* water) {
    FloatRect nextBounds = getCollisionBox();
    nextBounds.left += offsetX;
    nextBounds.top += offsetY;

    FloatRect legsBox = getLegHitbox();
    legsBox.left += offsetX;
    legsBox.top += offsetY;

    CircleHitbox rockHitbox = rock.getCollisionBoxData();

    //cout << str;

    if (circleIntersectsRect(rockHitbox.centerX, rockHitbox.centerY, rockHitbox.radius, nextBounds) ||
        circleIntersectsRect(rockHitbox.centerX, rockHitbox.centerY, rockHitbox.radius, legsBox)) {
        return true;
    }
    // Note: If one of the else ifs is evaluated as true the rest will be skipped
    else if (water != nullptr && nextBounds.intersects(water->getSprite().getGlobalBounds())) {
        return true;
    }
    else if (nextBounds.intersects(str.getSprite().getGlobalBounds())) {
        return true;
    }

    
    for (auto* waterBlock : waterBlocks.getTiles()) {
        if (nextBounds.intersects(waterBlock->getSprite().getGlobalBounds())) {
            return true;
        }
    }

    return false;
}

void Human::moveLeft(Rock& rock, Tileset& waterBlocks, Structure& str, Tile* water) {
    if (startX > 0 && !infantryIsColliding(-10, 0, rock, waterBlocks, str, water) && shouldBeAbleToMove) {
        startX -= 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveRight(Rock& rock, Tileset& waterBlocks, RenderWindow& window, Structure& str, Tile* water) {
    // Check if the character's right is colliding with the right edge of the window
    // getSize().x returns an unsigned int, since this variable isn't used to perform calculations, no need to save it is a float
    unsigned int rightEdge = window.getSize().x; 
    float soldierRight = sprite.getPosition().x + sprite.getGlobalBounds().width;
    if (soldierRight <= rightEdge && !infantryIsColliding(10, 0, rock, waterBlocks, str, water) && shouldBeAbleToMove) {
        startX += 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveDown(Rock& rock, Tileset& waterBlocks, RenderWindow& window, Structure& str, Tile* water) {
    // Check if the character's bottom is colliding with the bottom edge of the window
    // getSize().x returns an unsigned int, since this variable isn't used to perform calculations, no need to save it is a float
    unsigned int bottomEdge = window.getSize().y;
    float soldierBottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
    if (soldierBottom <= bottomEdge && !infantryIsColliding(0, 10, rock, waterBlocks, str, water) && shouldBeAbleToMove) {
        startY += 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveUp(Rock& rock, Tileset& waterBlocks, Structure& str, Tile* water) {
    if (startY > 0 && !infantryIsColliding(0, -10, rock, waterBlocks, str, water) && shouldBeAbleToMove) {
        startY -= 10;
        sprite.setPosition(startX, startY);
    }
}

// Functions related to NPC
NPC::NPC(const string& texturePath, float startX, float startY) : Entity(texturePath, startX, startY) {}

// This is the talk function for NPCs
void NPC::talk(RenderWindow& window, Human& player) {
    unsigned int bottomEdge = window.getSize().y; 
    DialogueBox dialogBox(window.getSize().x, 250.f, 0.f, bottomEdge - 250.f, Color(0, 0, 0, 128), Color::Red);
    dialogBox.setTitle("Tutorial NPC");
    dialogBox.setText("Hello Adventurer!");
    dialogBox.addCaption("Press Enter to continue...");

    window.draw(dialogBox);
}
