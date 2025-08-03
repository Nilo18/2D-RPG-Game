#pragma once
#ifndef GAME_GUI_H
#define GAME_GUI_H
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;


class DialogueBox {
private:
	RectangleShape box;
	Text text;
	Font font;
	//float width;
	//float height;
	//float positionX;
	//float positionY;
public:
	DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor);
};

#endif