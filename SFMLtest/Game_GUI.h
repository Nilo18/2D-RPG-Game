#pragma once
#ifndef GAME_GUI_H
#define GAME_GUI_H
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;
using namespace std;


class DialogueBox : public Drawable {
private:
	RectangleShape box;
	Text text;
	Font font;
	float outlineThickness = 2.0f;
public:
	DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor);
	virtual void draw(RenderTarget& target, RenderStates states) const override; // Overriden draw method from Drawable to make this custom class drawable
	void setText(const string& txt);
};

#endif