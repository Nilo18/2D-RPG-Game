#include "Game_GUI.h"

DialogueBox::DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor) {
	//this->width = width;
	//this->height = height;
	//this->positionX = positionX;
	//this->positionY = positionY;
	box.setSize(Vector2f(width, height));
	box.setPosition(positionX, positionY);
	box.setFillColor(fillColor);
	box.setOutlineColor(outlineColor);
}