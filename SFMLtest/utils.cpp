#include "utils.h"

namespace utilities {
	string wrapBoxText(const string& initialText, const Font& font, const RectangleShape& box) {
		string wrappedText;
		string word;
		float rightBoundary = box.getSize().x - 15.f;

		Text temp; // Local copy of text1 (main text)
		temp.setFont(font);
		temp.setCharacterSize(24);

		// Process each char of the txt
		istringstream iss(initialText);
		while (iss >> word) {
			// If we haven't already set the wrappedText set it to the word, else add an empty space between 
			string testLine = wrappedText.empty() ? word : wrappedText + " " + word;
			temp.setString(testLine); // Try using the test string
			// If the width of the temp exceeds the right boundary, move to the next line and continue
			if (temp.getLocalBounds().width > rightBoundary) {
				wrappedText += '\n' + word;
			}
			// Else try continuing regularly by adding the word and if the wrappedText isn't empty add in empty spaces
			else {
				if (!wrappedText.empty()) wrappedText += " ";
				wrappedText += word;
			}
		}

		return wrappedText;
	}

	void styleWidget(RectangleShape& shape, const WidgetStyle& style) {
		shape.setSize(style.size);
		shape.setPosition(style.position);
		shape.setFillColor(style.fillColor);
		shape.setOutlineColor(style.outlineColor);
		shape.setOutlineThickness(style.outlineThickness);
	}

	bool lineSegmentsIntersect(Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4) {
		// Calculate direction vectors
		Vector2f d1 = p2 - p1;
		Vector2f d2 = p4 - p3;

		// Calculate denominators for parametric equations
		float denominator = d1.x * d2.y - d1.y * d2.x;

		// If denominator is 0, lines are parallel
		if (abs(denominator) < 1e-10) return false;

		// Calculate parameters for intersection
		Vector2f d = p3 - p1;
		float t = (d.x * d2.y - d.y * d2.x) / denominator;
		float u = (d.x * d1.y - d.y * d1.x) / denominator;

		// Check if intersection point lies within both line segments
		return (t >= 0 && t <= 1 && u >= 0 && u <= 1);
	}

	bool pointInTriangle(const Vector2f& P, const Vector2f& A, const Vector2f& B, const Vector2f& C) {
		float s = A.y * C.x - A.x * C.y + (C.y - A.y) * P.x + (A.x - C.x) * P.y;
		float t = A.x * B.y - A.y * B.x + (A.y - B.y) * P.x + (B.x - A.x) * P.y;

		if ((s < 0) != (t < 0))
			return false;

		float area = -B.y * C.x + A.y * (C.x - B.x) + A.x * (B.y - C.y) + B.x * C.y;
		if (area < 0.0)
		{
			s = -s;
			t = -t;
			area = -area;
		}

		return (s > 0) && (t > 0) && (s + t <= area);
	}

	bool circleIntersectsRect(float cx, float cy, float radius, const FloatRect& rect) {
		// clamp restricts a value to a range
		// it takes (value, min, max) if value is less then min, min is returned
		// if value is greater than max, max is returned, otherwise value itself is returned
		// In this case this means that, if the X coordinate of the center of the circle is less than the left edge of the rectangle
		// It is left of the rectangle and the left edge is the closest collision point
		// If it is greater than the left edge, that means that it is right to the rectangle, and the right edge (left edge + width determines the right edge)
		// is the closest collision point, otherwise if the center is less than or equal to either rec.left (left edge) or rec.left + rec.width (right edge)
		// that means that it is already inside the rectangle, which means collision,
		// in this case the X coordinate of the center itself is the closest collision point
		// The same logic applies to the Y coordinate
		float closestX = clamp(cx, rect.left, rect.left + rect.width);
		float closestY = clamp(cy, rect.top, rect.top + rect.height);

		// distanceX is the distance horizontally between the center of the circle and the closest collision point
		// (It is the difference of the center and the closest collision point)
		// Same goes for distanceY
		float distanceX = cx - closestX;
		float distanceY = cy - closestY;

		// If the distance vector is less than or equal to the radius that means that there's no distance between the circle and the rectangle, i.e Collision
		// Because the circle is inside the rectangle, in this case true will be returned
		// Otherwise if the distance vector is greater than the radius there means there's still distance between the circle and the rectangle
		// in this case false will be returned
		// The formula itself is an alternative form of sqrt(distanceX^2+distanceY^2) <= radius
		// Both sides of the equation are squared to avoid sqrt() which is expensive in terms of performance
		return (distanceX * distanceX + distanceY * distanceY) <= (radius * radius);
	}

	bool pointInRect(const sf::Vector2f& P, const sf::FloatRect& rect) {
		return (P.x >= rect.left) && (P.x <= rect.left + rect.width) &&
			(P.y >= rect.top) && (P.y <= rect.top + rect.height);
	}

	sf::FloatRect unite(const sf::FloatRect& a, const sf::FloatRect& b) {
		// Find the minimum left and top coordinates
		float left = std::min(a.left, b.left);
		// Find the maximum right and bottom coordinates
		float top = std::min(a.top, b.top);
		// Calculate width and height based on the new left/top and the farthest right/bottom edges
		float right = std::max(a.left + a.width, b.left + b.width);
		// Calculate width and height based on the new left/top and the farthest right/bottom edges
		float bottom = std::max(a.top + a.height, b.top + b.height);

		// Return a new FloatRect that encompasses both rectangles
		return sf::FloatRect(left, top, right - left, bottom - top);
	}

};

