#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// This namespace will store all the utility functions like wrapping text
namespace utilities {
	string wrapBoxText(const string& initialText, const Font& font, const RectangleShape& box); // Method for dialog box text
	struct WidgetStyle {
		Vector2f size;
		Vector2f position;
		Color fillColor;
		Color outlineColor;
		float outlineThickness = 0.0f; // default
	}; // Struct to group the styling of GUI widgets like dialogue boxes together
	void styleWidget(RectangleShape& shape, const WidgetStyle& style);
};

#endif