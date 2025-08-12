#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
using namespace std;
using namespace sf;

class Structuress {
private:
	int P_x, P_y;
	Sprite building;
	Texture building_texture;
public:
	 Structuress()=default;
	 Structuress(int,int,const string &B);
	 void DrawBuilding(RenderWindow&window);

};
class obstacles: public Structuress{
public:
	obstacles( float& x, float& y, Sprite& ob, Texture& OB);
	void drawobstacle(RenderWindow& window);
};