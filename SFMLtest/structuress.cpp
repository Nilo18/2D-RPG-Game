#include "structuress.h"
//#include <iostream>

Structure::Structure( int X,int Y,const string &B)
{
	P_x = X;
	P_y = Y;
	if (!building_texture.loadFromFile(B))
		cerr << "clouldn't load from file";
	building.setTexture(building_texture);
	building.setPosition(P_x, P_y);
}

void Structure::DrawBuilding(RenderWindow& window) {
	window.draw(building);
}

FloatRect Structure::getCollisionBox() { return building.getGlobalBounds(); }
