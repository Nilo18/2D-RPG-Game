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
    // clamp restricts a value to a range
    // it takes (value, min, max) if value is less then min, min is returned
    // if value is greater than max, max is returned, otherwise value itself is returned
    // In this case this means that, if the X coordinate of the center of the circle is less than the left edge of the rectangle
    // It is left of the rectangle and the left edge is the closest collision point
    // If it is greater than the left edge, that means that it is right to the rectangle, and the right edge (left edge + width determines the right edge)
    // is the closest collision point, otherwise if the center is less than or equal to either rec.left (left edge) or rec.left + rec.width (right edge)
    // that means that it is already inside the rectangle, which means collision,
    // in this case the X coordinate of the center itself is the closest collision point
    // The same logic applies to the Y coordinate
    float closestX = clamp(cx, rect.left, rect.left + rect.width);
    float closestY = clamp(cy, rect.top, rect.top + rect.height);

    // distanceX is the distance horizontally between the center of the circle and the closest collision point
    // (It is the difference of the center and the closest collision point)
    // Same goes for distanceY
    float distanceX = cx - closestX;
    float distanceY = cy - closestY;

    // If the distance vector is less than or equal to the radius that means that there's no distance between the circle and the rectangle, i.e Collision
    // Because the circle is inside the rectangle, in this case true will be returned
    // Otherwise if the distance vector is greater than the radius there means there's still distance between the circle and the rectangle
    // in this case false will be returned
    // The formula itself is an alternative form of sqrt(distanceX^2+distanceY^2) <= radius
    // Both sides of the equation are squared to avoid sqrt() which is expensive in terms of performance
    return (distanceX * distanceX + distanceY * distanceY) <= (radius * radius);
}

// Functions related to Human
Human::Human(const string& texturePath, float startX, float startY) : Entity(texturePath, startX, startY) {}

void Human::shouldMove(bool val) {
    shouldBeAbleToMove = val;
}
    
FloatRect Human::getCollisionBox() {
    FloatRect boundingBox = sprite.getGlobalBounds();
    boundingBox.top += 20;
    boundingBox.left += 35;
    boundingBox.width -= 66;
    boundingBox.height -= 65;
    return boundingBox;
}

FloatRect Human::getLegHitbox() {
    FloatRect nextBounds = getCollisionBox();

    FloatRect legsBox = {
        nextBounds.left + 10.f,  // Offset from left
        nextBounds.top + 65.f,  // Offset from top
        37.f,                    // Fixed width
        37.f                     // Fixed height
    };

    return legsBox;
}

bool Human::infantryIsColliding(int offsetX, int offsetY, Rock& rock, Tileset& waterBlocks, House& house, Tile* water) {
    FloatRect nextBounds = getCollisionBox();
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
    // Note: If one of the else ifs is evaluated as true the rest will be skipped
    else if (water != nullptr && nextBounds.intersects(water->getSprite().getGlobalBounds())) {
        return true;
    }
    else if (nextBounds.intersects(house.getCollisionBox(118.f, 153.f, 229.f, 311.f)) || legsBox.intersects(house.getCollisionBox(118.f, 153.f, 229.f, 311.f))) {
        return true;
    }

    if (!waterBlocks.getTiles().empty()) {
        for (const auto& waterBlock : waterBlocks.getTiles()) {
            if (nextBounds.intersects(waterBlock->getSprite().getGlobalBounds())) {
                return true;
            }
        }
    }

    return false;
}

bool Human::shouldAppearBehind(House& house) {
    FloatRect nextBounds = getCollisionBox();
    FloatRect legsBox = getLegHitbox();
    TriangleHitbox houseRoofBox = house.getRoofCollisionBox(118.f, 153.f, 229.f, 311.f);
    array<Vector2f, 3> points = { houseRoofBox.topPoint, houseRoofBox.leftPoint, houseRoofBox.rightPoint };

    // Rectangle vertices
    array<Vector2f, 4> bodyVertices = {
        // Top left                                     // Top Right
        Vector2f(nextBounds.left, nextBounds.top), Vector2f(nextBounds.left + nextBounds.width, nextBounds.top),
        // Bottom left                                                             // Bottom Right
        Vector2f(nextBounds.left, nextBounds.top + nextBounds.height), Vector2f(nextBounds.left + nextBounds.width, nextBounds.top + nextBounds.height)
    };

    // Legs rectangle vertices
    array<Vector2f, 4> legVertices = { 
        // Top left                                     // Top Right
        Vector2f(legsBox.left, legsBox.top), Vector2f(legsBox.left + legsBox.width, legsBox.top),
        // Bottom left                                                             // Bottom Right
        Vector2f(legsBox.left, legsBox.top + legsBox.height), Vector2f(legsBox.left + legsBox.width, legsBox.top + legsBox.height),
    };

    // Check if triangle vertices are inside either rectangle
    if (!points.empty()) {
        for (const auto& point : points) {
            // If any of the vertices are inside the left edge or the right edge, collision detected
            // Same goes for the legs
            if (((point.x >= nextBounds.left && point.x <= nextBounds.left + nextBounds.width)
                && (point.y >= nextBounds.top && point.y <= nextBounds.top + nextBounds.height)) ||
                ((point.x >= legsBox.left && point.x <= legsBox.left + legsBox.width)
                    && (point.y >= legsBox.top && point.y <= legsBox.top + legsBox.height))) {
                return true;
            }
        }
    }

    // Check if body vertices are inside the triangle
    if (!bodyVertices.empty()) {
        for (const auto& vertex : bodyVertices) {
            if (utilities::pointInTriangle(vertex, points[0], points[1], points[2])) return true;
        }
    }

    // Check if leg vertices are inside the triangle
    if (!legVertices.empty()) {
        for (const auto& vertex : legVertices) {
            if (utilities::pointInTriangle(vertex, points[0], points[1], points[2])) return true;
        }
    }

    // The pair of vertices will make up the edges (These are the body hitbox edges)
    array<pair<Vector2f, Vector2f>, 4> rectEdges = {
        make_pair(bodyVertices[0], bodyVertices[1]),
        make_pair(bodyVertices[1], bodyVertices[3]),
        make_pair(bodyVertices[3], bodyVertices[2]), //botRight, botLeft
        make_pair(bodyVertices[0], bodyVertices[2]) //botLeft topLeft
    };

    // Same goes for the legs
    array<pair<Vector2f, Vector2f>, 4> legsEdges = {
        make_pair(legVertices[0], legVertices[1]), // topLeft, topRight
        make_pair(legVertices[1], legVertices[3]), // legsTopRight, legsBottomRight
        make_pair(legVertices[3], legVertices[2]), // legsBottomRight, legsBottomLeft
        make_pair(legVertices[0], legVertices[2])  // legsBottomLeft, legsTopLeft
    };

    // Same goes for the triangle (Part of the roof)
    array<pair<Vector2f, Vector2f>, 3> triEdges = {
        make_pair(points[0], points[1]),
        make_pair(points[1], points[2]),
        make_pair(points[2], points[1])
    };

    // Check intersections between rectangle edges and triangle edges
    for (const auto& rectEdge : rectEdges) {
        for (const auto& triEdge : triEdges) {
            if (utilities::lineSegmentsIntersect(rectEdge.first, rectEdge.second, triEdge.first, triEdge.second)) return true; // Collision detected
        }
    }

    // Check intersections between legs rectangle edges and triangle edges
    for (const auto& legEdge : legsEdges) {
        for (const auto& triEdge : triEdges) {
            if (utilities::lineSegmentsIntersect(legEdge.first, legEdge.second, triEdge.first, triEdge.second)) return true; // Collision detected
        }
    }

    return false; // Return false by default (no collision)
}

void Human::moveLeft(Rock& rock, Tileset& waterBlocks, House& house, Tile* water) {
    if (startX > 0 && !infantryIsColliding(-10, 0, rock, waterBlocks, house, water) && shouldBeAbleToMove) {
        startX -= 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveRight(Rock& rock, Tileset& waterBlocks, RenderWindow& window, House& house, Tile* water) {
    // Check if the character's right is colliding with the right edge of the window
    // getSize().x returns an unsigned int, since this variable isn't used to perform calculations, no need to save it is a float
    unsigned int rightEdge = window.getSize().x; 
    float soldierRight = sprite.getPosition().x + sprite.getGlobalBounds().width;
    if (soldierRight <= rightEdge && !infantryIsColliding(10, 0, rock, waterBlocks, house, water) && shouldBeAbleToMove) {
        startX += 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveDown(Rock& rock, Tileset& waterBlocks, RenderWindow& window, House& house, Tile* water) {
    // Check if the character's bottom is colliding with the bottom edge of the window
    // getSize().x returns an unsigned int, since this variable isn't used to perform calculations, no need to save it is a float
    unsigned int bottomEdge = window.getSize().y;
    float soldierBottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
    if (soldierBottom <= bottomEdge && !infantryIsColliding(0, 10, rock, waterBlocks, house, water) && shouldBeAbleToMove) {
        startY += 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveUp(Rock& rock, Tileset& waterBlocks, House& house, Tile* water) {
    if (startY > 0 && !infantryIsColliding(0, -10, rock, waterBlocks, house, water) && shouldBeAbleToMove) {
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
