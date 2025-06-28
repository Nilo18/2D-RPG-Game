#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
using namespace sf;
using namespace std;
using namespace std::filesystem;
class Water;
class WaterGroup;
class Rock;

const int TILE_SIZE = 64;
const int TILES_X = 1000 / TILE_SIZE;  // = 15
const int TILES_Y = 800 / TILE_SIZE;   // = 12

// Load tilemap from file into a 2D vector
vector<vector<int>> loadTileMap(const string& filename) {
    try {
        ifstream file(filename);
        if (!file) throw std::runtime_error("Failed to open tilemap file: " + filename);
        vector<std::vector<int>> map;
        string line;

        while (getline(file, line)) {
            vector<int> row;
            istringstream iss(line);
            int tileId;
            while (iss >> tileId) {
                row.push_back(tileId);
            }
            map.push_back(row);
        }

        return map;
    }
    catch (exception& Exception) {
        cout << Exception.what();
        return {};
    };


}
//class TileMap {
//private:
//    int map[tilesY][tilesX];
//
//};

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

Grass::Grass(const string& texturePath, int startX, int startY) {
    if (!grassTexture.loadFromFile(texturePath)) {
        throw runtime_error("Couldn't load grass texture.");
    }
    grassSprite.setTexture(grassTexture);
    this->startX = startX;
    this->startY = startY;
    grassSprite.setPosition(this->startX, this->startY);
}

const Sprite& Grass::getSprite() { return grassSprite; }

class GrassGroup : public Drawable{
private:
    vector<Grass*> grassTiles;
public:
    // We're using floats because setPosition takes floats plus it allows for smoother movement if ever needed
    GrassGroup(const string& texturePath, float startX, float startY, int rowsToSpan, int colsToSpan);
    ~GrassGroup();
    const vector <Grass*>& getGrassTiles() const;
    virtual void draw(RenderTarget& target, RenderStates states) const override; // This method will be called internally when drawing the object
};

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

Water::Water(const string& texturePath, int startX, int startY) {
    if (!waterTexture.loadFromFile(texturePath)) {
        throw runtime_error("Couldn't load water texture.");
    }
    waterSprite.setTexture(waterTexture);
    this->startX = startX;
    this->startY = startY;
    waterSprite.setPosition(this->startX, this->startY);
}

const Sprite& Water::getSprite() const { return waterSprite; }

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
//const Sprite& WaterGroup::getSprite() const {return }

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

Rock::Rock(const string& texturePath, float startX, float startY) {
    if (!rockTexture.loadFromFile(texturePath)) {
        throw runtime_error("Failed to load rock texture.");
    }
    rockSprite.setTexture(rockTexture);
    this->startX = startX;
    this->startY = startY;
    rockSprite.setPosition(this->startX, this->startY);
}

const Sprite& Rock::getSprite() const { return rockSprite; }

FloatRect Rock::getCollisionBox() {
    //FloatRect hitbox = rockSprite.getGlobalBounds();
    //hitbox.top += 14;
    //hitbox.left += 5;
    //hitbox.width -= 10;
    //hitbox.height -= 26;
    //return hitbox;
    float diameter = rockSprite.getGlobalBounds().width * 0.5f;
    float radius = diameter / 2.f;
    
    float centerX = rockSprite.getPosition().x + rockSprite.getGlobalBounds().width / 2.f;
    float centerY = rockSprite.getPosition().y + rockSprite.getGlobalBounds().height / 2.f;

    centerY += 1.1;

    return FloatRect(centerX - radius, centerY - radius, diameter, diameter);
}

CircleHitbox Rock::getCollisionBoxData() {
    float diameter = rockSprite.getGlobalBounds().width * 0.6f;
    float radius = diameter / 2.f;

    float centerX = rockSprite.getPosition().x + rockSprite.getGlobalBounds().width / 2.f;
    float centerY = rockSprite.getPosition().y + rockSprite.getGlobalBounds().height / 2.f;

    centerY += 1.1;

    return { centerX, centerY, radius };
}


void Rock::draw(RenderWindow& window) {
    window.draw(rockSprite);
}

class Infantry {
private:
    Sprite infSprite;
    Texture infTexture;
    int startX;
    int startY;
public:
    Infantry(const string& texturePath, int startX, int startY);
    void moveLeft(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    void moveRight(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    void moveDown(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    void moveUp(Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr);
    void draw(RenderWindow& window);
    void setScale(float scaleX, float scaleY);
    bool infantryIsColliding(int offsetX, int offsetY, Rock& rock, WaterGroup& waterBlocks, Water* water = nullptr); // Take all collidable objects as parameters to check for each 
    FloatRect getCollisionBox(); // We don't return by a const reference here because we're returning a temporary variable, created in the body
    FloatRect getLegHitbox();
};

Infantry::Infantry(const string& texturePath, int startX, int startY) {
    this->startX = startX;
    this->startY = startY;
    if (!infTexture.loadFromFile(texturePath)) {
        throw runtime_error("Failed to load texture");
    }
    infSprite.setTexture(infTexture);
    infSprite.setPosition(this->startX, this->startY);
}

FloatRect Infantry::getCollisionBox() {
    FloatRect boundingBox = infSprite.getGlobalBounds();
    boundingBox.top += 20;
    boundingBox.left += 22;
    boundingBox.width -= 66;
    boundingBox.height -= 65;
    return boundingBox;
}

FloatRect Infantry::getLegHitbox() {
    FloatRect nextBounds = getCollisionBox();

    FloatRect legsBox = {
        nextBounds.left + 14.f,  // Offset from left
        nextBounds.top + 65.f,  // Offset from top
        37.f,                    // Fixed width
        35.f                     // Fixed height
    };

    return legsBox;
}

// Function for checking rock's collision (circular hitbox) with player (rectangle hitbox)
bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect) {
    float closestX = clamp(cx, rect.left, rect.left + rect.width);
    float closestY = clamp(cy, rect.top, rect.top + rect.height);

    float distanceX = cx - closestX;
    float distanceY = cy - closestY;

    return (distanceX * distanceX + distanceY * distanceY) < (radius * radius);
}

bool Infantry::infantryIsColliding(int offsetX, int offsetY, Rock& rock, WaterGroup& waterBlocks, Water* water) {
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

void Infantry::moveLeft(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startX > 0 && !infantryIsColliding(-5, 0, rock, waterBlocks, water)) {
        startX -= 5;   
        infSprite.setPosition(startX, startY);
    }
}

void Infantry::moveRight(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startX <= 1000 && !infantryIsColliding(5, 0, rock, waterBlocks, water)) {
        startX += 5;
        infSprite.setPosition(startX, startY);
    }
}

void Infantry::moveDown(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startY <= 800 && !infantryIsColliding(0, 5, rock, waterBlocks, water)) {
        startY += 5;
        infSprite.setPosition(startX, startY);
    }
}

void Infantry::moveUp(Rock& rock, WaterGroup& waterBlocks, Water* water) {
    if (startY > 0 && !infantryIsColliding(0, -5, rock, waterBlocks, water)) {
        startY -= 5;
        infSprite.setPosition(startX, startY);
    }
}

void Infantry::draw(RenderWindow& window) {
    window.draw(infSprite);
}

void Infantry::setScale(float scaleX, float scaleY) {
    infSprite.setScale(scaleX, scaleY);
}

int main() {
    // Create a window with title and size
    //std::cout << "Working directory is: " << filesystem::current_path() << std::endl;
    RenderWindow window(sf::VideoMode(992, 800), "SFML Test Window");
     
    // To determine how many blocks we have to draw we divide the width and height by the tile size and add 1
    // First value of Grass/Water Group takes the texture, second and third starting x and y positions and fourth and fifth the area on which they should span
    GrassGroup grassBlocks("assets/grass2.png", 0, 0, window.getSize().y / TILE_SIZE + 1, window.getSize().x / TILE_SIZE + 1);
    WaterGroup waterBlocks("assets/water.png", 0, 0, 3, 5);
    Rock rock("assets/rock2.png", 500, 500);
    Infantry soldier("assets/infantry2.png", 100, 250);
    soldier.setScale(2.f, 2.f);  // doubles width and height

    //This block of code is for debugging only
    /***************************************************/
    //FloatRect bounds = rock.getCollisionBox();

    //CircleHitbox hitbox = rock.getCollisionBoxData();

    //CircleShape debugBox;
    //debugBox.setRadius(hitbox.radius);
    //debugBox.setOrigin(hitbox.radius, hitbox.radius);
    //debugBox.setPosition(hitbox.centerX, hitbox.centerY);
    //debugBox.setFillColor(Color::Transparent);
    //debugBox.setOutlineColor(Color::Red);
    //debugBox.setOutlineThickness(1.f); // Required to see the red outline

    //sf::FloatRect soldierBounds = soldier.getCollisionBox();  // Or next position with offset
    //sf::RectangleShape soldierBox;
    //soldierBox.setPosition(soldierBounds.left, soldierBounds.top);
    //soldierBox.setSize({ soldierBounds.width, soldierBounds.height });
    //soldierBox.setOutlineColor(sf::Color::Blue);
    //soldierBox.setOutlineThickness(1.f);
    //soldierBox.setFillColor(sf::Color::Transparent);

    //FloatRect soldierLegHitbox = soldier.getLegHitbox();
    //RectangleShape legHitbox;
    //legHitbox.setPosition(soldierLegHitbox.left, soldierLegHitbox.top);
    //legHitbox.setSize({ soldierLegHitbox.width, soldierLegHitbox.height });
    //legHitbox.setOutlineColor(sf::Color::Yellow);
    //legHitbox.setOutlineThickness(1.f);
    //legHitbox.setFillColor(sf::Color::Transparent);
    /***************************************************/
    // Main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            // Close window on request
            switch (event.type) {
                case Event::Closed:
                    window.close(); break;
                     //If key is pressed check which one is it
                case Event::KeyPressed:
                    switch (event.key.code) {
                    case Keyboard::A:
                        soldier.moveLeft(rock, waterBlocks); break;
                    case Keyboard::D:
                        soldier.moveRight(rock, waterBlocks); break;
                    case Keyboard::S:
                        soldier.moveDown(rock, waterBlocks); break;
                    case Keyboard::W:   
                        soldier.moveUp(rock, waterBlocks); break;
                    }
            }
                
        }
        // Clear the screen with black
        window.clear(Color(105, 255, 255, 255)); // Clear old frame
        window.draw(grassBlocks);
        window.draw(waterBlocks);
        rock.draw(window);
        //window.draw(debugBox);
        soldier.draw(window);
        //window.draw(soldierBox);
        //window.draw(legHitbox);
        window.display(); // Tell the app that the window is done drawing
    }

}

