#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

// This namespace will store all the utility functions like wrapping text and styling a widget
namespace utilities {
	string wrapBoxText(const string& initialText, const Font& font, const RectangleShape& box); // Method for dialog box text
	// Struct to group the styling of GUI widgets like dialogue boxes together
	struct WidgetStyle {
		Vector2f size;
		Vector2f position;
		Color fillColor;
		Color outlineColor;
		float outlineThickness = 0.0f; // default
	}; 
	void styleWidget(RectangleShape& shape, const WidgetStyle& style);
	// Struct to group class object properties like coordinates, textures and sprites together
	//struct ObjectProperties {
	//	float objectX = 0.0f; // class object's X coordinate
	//	float objectY = 0.0f; // class object's Y coordinate
	//	float givenX = 0.0f; // given X coordinate to initialize the class object's coordinate above
	//	float givenY = 0.0f; // given Y coordinate to initialize the class object's coordinate above
	//	Texture texture; // class object's texture
	//	Sprite sprite; // class object's sprite
	//};
	//// This function will load the texture and set the starting coordinates for class constructors
	//template <typename T>
	//void initializeObject(const string& texturePath, T& object, float givenX, float givenY); 
};

#endif