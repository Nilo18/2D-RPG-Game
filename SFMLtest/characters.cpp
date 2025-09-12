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

//FloatRect Entity::getCollisionBox() {
//    return sprite.getGlobalBounds();
//}

void Entity::setScale(float scaleX, float scaleY) {
    sprite.setScale(scaleX, scaleY);
}

// Functions related to Human
Human::Human(const string& texturePath, float startX, float startY) : Entity(texturePath, startX, startY) {}

void Human::shouldMove(bool val) {
    shouldBeAbleToMove = val;
}

const Hitbox* Human::getCollisionBox(float left, float top, float width, float height) {
    FloatRect boundingBox = sprite.getGlobalBounds();
    boundingBox.top += 20;  
    boundingBox.left += 35;
    boundingBox.width -= 66;
    boundingBox.height -= 65;
    //boundingBox.top += hitboxOffset.top;
    //boundingBox.left += hitboxOffset.left;
    //boundingBox.width -= hitboxOffset.width;
    //boundingBox.height -= hitboxOffset.height;
    collisionBox = RectangleHitbox(boundingBox.left, boundingBox.top, boundingBox.width, boundingBox.height);
    return &collisionBox;
}

const Hitbox* Human::getLegHitbox(float left, float top, float width, float height) {
    const Hitbox* nextBounds = getCollisionBox();

    const RectangleHitbox* recHitbox = dynamic_cast<const RectangleHitbox*>(nextBounds);

    //if (!recHitbox) {
    //    return FloatRect{};
    //}

    FloatRect legsBox = {
        recHitbox->rect.left + 10.f,  // Offset from left
        recHitbox->rect.top + 65.f,  // Offset from top
        37.f,                    // Fixed width
        37.f                     // Fixed height
    };

	legHitbox = RectangleHitbox(legsBox.left, legsBox.top, legsBox.width, legsBox.height);
    return &legHitbox;
}

bool Human::shouldAppearBehind(const std::shared_ptr<House>& house) {
    // Avoid dynamic_cast because since we have to return bool if we can't return FloatRect{} in case dynamic_cast returns nullptr
    FloatRect nextBounds = getCollisionBox()->getRect();
    FloatRect legsBox = getLegHitbox()->getRect();

    TriangleHitbox houseRoofBox = house->getRoofCollisionBox();
    array<Vector2f, 3> points = { houseRoofBox.topPoint, houseRoofBox.leftPoint, houseRoofBox.rightPoint };

    TriangleHitbox houseChimneyBox = house->getChimneyCollisionBox();
    array<Vector2f, 3> chimneyPoints = { houseChimneyBox.topPoint, houseChimneyBox.leftPoint, houseChimneyBox.rightPoint };

    // Rectangle vertices
    array<Vector2f, 4> bodyVertices = {
        // Top left                                     // Top Right
        Vector2f(nextBounds.left, nextBounds.top), Vector2f(nextBounds.left + nextBounds.width, nextBounds.top),
        // Bottom left                                                             
        Vector2f(nextBounds.left, nextBounds.top + nextBounds.height),
        // Bottom Right
        Vector2f(nextBounds.left + nextBounds.width, nextBounds.top + nextBounds.height)
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

    if (!chimneyPoints.empty()) {
        for (const auto& point : chimneyPoints) {
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
            // For the roof
            if (utilities::pointInTriangle(vertex, points[0], points[1], points[2])) return true;
            // For the chimney
            if (utilities::pointInTriangle(vertex, chimneyPoints[0], chimneyPoints[1], chimneyPoints[2])) return true;
        }
    }

    // Check if leg vertices are inside the triangle
    if (!legVertices.empty()) {
        for (const auto& vertex : legVertices) {
            // For the roof
            if (utilities::pointInTriangle(vertex, points[0], points[1], points[2])) return true;
            // For the chimney
            if (utilities::pointInTriangle(vertex, chimneyPoints[0], chimneyPoints[1], chimneyPoints[2])) return true;
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

    // And for the chimney
    array<pair<Vector2f, Vector2f>, 3> chimneyTriEdges = {
        make_pair(chimneyPoints[0], chimneyPoints[1]),
        make_pair(chimneyPoints[1], chimneyPoints[2]),
        make_pair(chimneyPoints[2], chimneyPoints[1])
    };


    // Check intersections between rectangle edges and triangle edges
    if (!rectEdges.empty()) {
        for (const auto& rectEdge : rectEdges) {
            for (const auto& triEdge : triEdges) {
                if (utilities::lineSegmentsIntersect(rectEdge.first, rectEdge.second, triEdge.first, triEdge.second)) return true; // Collision detected
            }

            for (const auto& chimneyTriEdge : chimneyTriEdges) {
                if (utilities::lineSegmentsIntersect(rectEdge.first, rectEdge.second, chimneyTriEdge.first, chimneyTriEdge.second)) return true; // Collision detected
            }
        }
    }

    // Check intersections between legs rectangle edges and triangle edges
    if (!legsEdges.empty()) {
        for (const auto& legEdge : legsEdges) {
            for (const auto& triEdge : triEdges) {
                if (utilities::lineSegmentsIntersect(legEdge.first, legEdge.second, triEdge.first, triEdge.second)) return true; // Collision detected
            }

            for (const auto& chimneyTriEdge : chimneyTriEdges) {
                if (utilities::lineSegmentsIntersect(legEdge.first, legEdge.second, chimneyTriEdge.first, chimneyTriEdge.second)) return true; // Collision detected
            }
        }
    }

    return false; // Return false by default (no collision)
}

void Human::moveLeft() {
    if (startX > 0 && shouldBeAbleToMove) {
        startX -= 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveRight(RenderWindow& window) {
    // Check if the character's right is colliding with the right edge of the window
    // getSize().x returns an unsigned int, since this variable isn't used to perform calculations, no need to save it is a float
    unsigned int rightEdge = window.getSize().x;
    float soldierRight = sprite.getPosition().x + sprite.getGlobalBounds().width;
    if (soldierRight <= rightEdge && shouldBeAbleToMove) {
        startX += 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveDown(RenderWindow& window) {
    // Check if the character's bottom is colliding with the bottom edge of the window
    // getSize().x returns an unsigned int, since this variable isn't used to perform calculations, no need to save it is a float
    unsigned int bottomEdge = window.getSize().y;
    float soldierBottom = sprite.getPosition().y + sprite.getGlobalBounds().height;
    if (soldierBottom <= bottomEdge && shouldBeAbleToMove) {
        startY += 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::moveUp() {
    if (startY > 0 && shouldBeAbleToMove) {
        startY -= 10;
        sprite.setPosition(startX, startY);
    }
}

void Human::setHitboxOffset(float left, float top, float width, float height) {
	hitboxOffset = FloatRect(left, top, width, height);
}

// Functions related to NPC
NPC::NPC(const string& texturePath, float startX, float startY) : Entity(texturePath, startX, startY) {}

// This is the talk function for NPCs
void NPC::talk(RenderWindow& window) {
    unsigned int bottomEdge = window.getSize().y;
    DialogueBox dialogBox(window.getSize().x, 250.f, 0.f, bottomEdge - 250.f, Color(0, 0, 0, 128), Color::Red);
    dialogBox.setTitle("Tutorial NPC");
    dialogBox.setText("Hello Adventurer!");
    dialogBox.addCaption("Press Enter to continue...");

    window.draw(dialogBox);
}

const Hitbox* NPC::getCollisionBox(float left, float top, float width, float height ) {
    FloatRect npcBounds = sprite.getGlobalBounds();
    npcHitbox = RectangleHitbox(npcBounds.left, npcBounds.top, npcBounds.width, npcBounds.height);
    return &npcHitbox;
}


void NPC::setHitboxOffset(float left, float top, float width, float height) {
    hitboxOffset = FloatRect(left, top, width, height);
}
