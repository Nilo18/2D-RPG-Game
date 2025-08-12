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
	Text mainText,caption,title; // caption is for notes like "Press Enter to continue" and title is to determine who's speaking
	Font boldPixelSans, regularPixelSans;
	float outlineThickness = 2.0f;
public:
	DialogueBox(float width, float height, float positionX, float positionY, Color fillColor, Color outlineColor);
	virtual void draw(RenderTarget& target, RenderStates states) const override; // Overriden draw method from Drawable to make this custom class drawable
	void setTitle(const string& title);
	void setText(const string & txt);
	void addCaption(const string& tx);
};

#endif