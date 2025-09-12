#pragma once
#include <SFML/Graphics.hpp>

namespace utilities {
	// Helper function to check if the points of a rectangle are in a triangle (Needed for rectangle and triangle collision detection)
	inline bool pointInTriangle(const sf::Vector2f& P, const sf::Vector2f& A, const sf::Vector2f& B, const sf::Vector2f& C) {
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
}