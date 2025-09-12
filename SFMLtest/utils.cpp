#include "utils.h"
//#include "CollisionManager.h"

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

	RectangleShape createRectangleDebugBox(const FloatRect& recHitbox, const Color& outlineColor, const Color& fillColor, const float outlineThickness) {
		RectangleShape debugBox;
		debugBox.setPosition({ recHitbox.left, recHitbox.top });
		debugBox.setSize({ recHitbox.width, recHitbox.height });
		debugBox.setOutlineColor(outlineColor);
		debugBox.setFillColor(fillColor);
		debugBox.setOutlineThickness(outlineThickness);
		return debugBox;
	}

	CircleShape createCircleDebugBox(const CircleHitbox& cirHitbox, const Color& outlineColor, const Color& fillColor, const float outlineThickness) {
		CircleShape debugBox;
		debugBox.setRadius(cirHitbox.radius);
		debugBox.setOrigin(cirHitbox.radius, cirHitbox.radius);
		debugBox.setPosition(cirHitbox.centerX, cirHitbox.centerY);
		debugBox.setFillColor(Color::Transparent);
		debugBox.setOutlineColor(Color::Red);
		debugBox.setOutlineThickness(1.f); // Required to see the red outline
		return debugBox;
	}

	ConvexShape createTriangleDebugBox(const TriangleHitbox& trHitbox, const Color& outlineColor, const Color& fillColor, const float outlineThickness) {
		ConvexShape debugBox;
		debugBox.setPointCount(3);
		debugBox.setPoint(0, trHitbox.topPoint); // The first parameter is index of the vertex (point)
		debugBox.setPoint(1, trHitbox.leftPoint);
		debugBox.setPoint(2, trHitbox.rightPoint);
		debugBox.setOutlineColor(outlineColor);
		debugBox.setFillColor(fillColor);
		debugBox.setOutlineThickness(outlineThickness);
		return debugBox;
	}
};

