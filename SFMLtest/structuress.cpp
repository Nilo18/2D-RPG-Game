#include "structuress.h"
//#include <iostream>

// Structure functions
Structure::Structure(const string& B, float X, float Y)
{
	P_x = X;
	P_y = Y;
	if (!building_texture.loadFromFile(B))
		cerr << "clouldn't load from file";
	building.setTexture(building_texture);
	building.setPosition(P_x, P_y);
}

void Structure::draw(RenderTarget& target, RenderStates states) const {
	target.draw(building, states);
}

FloatRect Structure::getCollisionBox(float left, float top, float width, float height) { return building.getGlobalBounds(); }
const Sprite& Structure::getSprite() const { return building; }

// House functions
House::House(const string& texturePath, float startX, float startY) : Structure(texturePath, startX, startY) {}

FloatRect House::getCollisionBox(float left, float top, float width, float height) {
	// Later on we can make this method take parameters to adjust the hitbox by the given values, like houseHitbox.left += givenLeft, houseHitbox.top += givenTop
	FloatRect houseHitbox = building.getGlobalBounds();
	houseHitbox.left += left;
	houseHitbox.top += top;
	houseHitbox.width -= width;
	houseHitbox.height -= height;
	return houseHitbox;
}

TriangleHitbox House::getRoofCollisionBox(float left, float top, float width, float height) {
	// Pass the given values to the base collision box to initialize it
	FloatRect bounds = getCollisionBox(left, top, width, height); // Lower part of the roof collision box

	Vector2f topPoint(bounds.left + bounds.width / 2.f, bounds.top - 130.f);
	Vector2f leftPoint(bounds.left, bounds.top);
	Vector2f rightPoint(bounds.left + bounds.width, bounds.top);

	return { topPoint, leftPoint, rightPoint };
}
