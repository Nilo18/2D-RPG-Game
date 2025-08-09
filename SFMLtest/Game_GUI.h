#pragma once
#ifndef GAME_GUI_H
#define GAME_GUI_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include "utils.h"
using namespace sf;
using namespace std;


class DialogueBox : public Drawable {
private:
	RectangleShape box;
	Text text1,text2,text3;
	Font font ,font1;
	float outlineThickness = 2.0f;
public:
	DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor);
	virtual void draw(RenderTarget& target, RenderStates states) const override; // Overriden draw method from Drawable to make this custom class drawable
	void setText(const string & txt);
	void addtext(const string& tx);
};

#endif