#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include "CircleIntersectsRect.h"
#include "PointsInRect.h"
#include "CircleHitbox.h"
#include "TriangleHitbox.h"
#include "PointsInTriangle.h"
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
	//bool pointInRect(const sf::Vector2f& P, const sf::FloatRect& rect); 
	sf::FloatRect unite(const sf::FloatRect& a, const sf::FloatRect& b); 
	// Helper functions for drawing hitboxes for debugging
	RectangleShape createRectangleDebugBox(const FloatRect& recHitbox, const Color& outlineColor, const Color& fillColor, const float outlineThickness);
	CircleShape createCircleDebugBox(const CircleHitbox& cirHitbox, const Color& outlineColor, const Color& fillColor, const float outlineThickness);
	ConvexShape createTriangleDebugBox(const TriangleHitbox& trHitbox, const Color& outlineColor, const Color& fillColor, const float outlineThickness);
};

#endif