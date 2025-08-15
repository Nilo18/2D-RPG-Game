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

FloatRect Structure::getCollisionBox() { return building.getGlobalBounds(); }
const Sprite& Structure::getSprite() const { return building; }

// House functions
House::House(const string& texturePath, float startX, float startY) : Structure(texturePath, startX, startY) {}

FloatRect House::getCollisionBox() {
	// Later on we can make this method take parameters to adjust the hitbox by the given values, like houseHitbox.left += givenLeft, houseHitbox.top += givenTop
	FloatRect houseHitbox = building.getGlobalBounds();
	houseHitbox.left += 118.f;
	houseHitbox.top += 153.f;
	houseHitbox.width -= 229.f;
	houseHitbox.height -= 311.f;
	return houseHitbox;
}
