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
	// Helper function to check if two line segments intersect (Needed for rectangle and triangle collision detection)
	bool lineSegmentsIntersect(Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4);
	// Helper function to check if the points of a rectangle are in a triangle (Needed for rectangle and triangle collision detection)
	bool pointInTriangle(const Vector2f& P, const Vector2f& A, const Vector2f& B, const Vector2f& C);
	bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect);
	// Helper function to check if the points of a triangle are in a rectangle (Needed for rectangle and triangle collision detection)
	bool pointInRect(const sf::Vector2f& P, const sf::FloatRect& rect); 
	sf::FloatRect unite(const sf::FloatRect& a, const sf::FloatRect& b); 
};

#endif