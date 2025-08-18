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

	//template <typename T>
	//void initializeObject(const string& texturePath, T& object, float givenX, float givenY) {
	//	//if (!objectProps.texture.loadFromFile(texturePath)) {
	//	//	throw runtime_error("Couldn't load texture");
	//	//}
	//	//objectProps.sprite.setTexture(objectProps.texture);
	//	//objectProps.objectX = objectProps.givenX;
	//	//objectProps.objectY = objectProps.givenY;
	//	//cout << objectProps.objectX << " " << objectProps.objectY << endl;
	//	//objectProps.sprite.setPosition(objectProps.objectX, objectProps.objectY);
	//	if (!object.texture.loadFromFile(texturePath)) {
	//		throw runtime_error("Couldn't load file");
	//	}
	//	object.sprite.setTexture(object.texture);
	//	object.getX() = givenX;
	//	object.getY() = givenY;
	//	object.sprite.setPosition(object.startX, object.startY);

	//}

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

};

