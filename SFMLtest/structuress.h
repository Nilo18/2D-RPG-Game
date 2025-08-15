#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
using namespace std;
using namespace sf;

class Structure {
private:
	int P_x, P_y;
	Sprite building;
	Texture building_texture;
public:
	 Structure()=default;
	 Structure(int,int,const string &B);
	 void DrawBuilding(RenderWindow&window);
	 virtual FloatRect getCollisionBox(); // Keep it virtual so other objects can override it to fit their own sizes
	 const Sprite& getSprite() const { return building; }
};

class obstacles: public Structure {
public:
	obstacles( float& x, float& y, Sprite& ob, Texture& OB);
	void drawobstacle(RenderWindow& window);
};