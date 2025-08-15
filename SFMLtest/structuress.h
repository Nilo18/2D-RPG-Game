#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
using namespace std;
using namespace sf;

// Base class for all buildings
class Structure : public Drawable {
protected:
	float P_x, P_y;
	Sprite building;
	Texture building_texture;
public:
	Structure()=default;
	// setPosition() floatebs igebs da intebi agar miagebino konstruqtorebshi
	Structure(const string& B, float, float);
	// This overrides the official draw() method and allows the window to draw the given object
	// RenderTarget is anything that can be drawn, RanderStates defines the states that the object can be drawn in like rotated
	// We override it and make it virtual as well so other subclasses which will inherit from it can override it as well
	virtual void draw(RenderTarget& target, RenderStates states) const override; 
	virtual FloatRect getCollisionBox(); // Keep it virtual so other objects can override it to fit their own sizes
	const Sprite& getSprite() const;
};

class Obstacles: public Structure {
public:
	// ricxvebs tu ar cvli da abruneb referensebit nu igeb patara obieqtebia isedac da ar ari sachiro
	Obstacles( float x, float y, Sprite& ob, Texture& OB);
	//void drawobstacle(RenderWindow& window); // es zedmetia imito ro Structure-is draw()-c sakmarisia
};

class House : public Structure {
public:
	House(const string& texturePath, float startX, float startY);
	FloatRect getCollisionBox() override;
	ConvexShape getRoofCollisionBox();
};