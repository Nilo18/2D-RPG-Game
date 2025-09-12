#pragma once
#include <SFML/Graphics.hpp>

namespace utilities {
	// Helper function to check if the points of a triangle are in a rectangle (Needed for rectangle and triangle collision detection)
	inline bool pointInRect(const sf::Vector2f& P, const sf::FloatRect& rect) {
		return (P.x >= rect.left) && (P.x <= rect.left + rect.width) &&
			(P.y >= rect.top) && (P.y <= rect.top + rect.height);
	}
}