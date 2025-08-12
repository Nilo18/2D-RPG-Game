#include "structuress.h"

Structuress::Structuress( int X,int Y,const string &B)
{
	P_x = X;
	P_y = Y;
	if (!building_texture.loadFromFile(B))
		cerr << "clouldn't load from file";
	building.setTexture(building_texture);
	building.setPosition(P_x, P_y);
}
void Structuress::DrawBuilding(RenderWindow& window) {
	window.draw(building);
}
