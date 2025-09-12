#pragma once
#include <SFML/Graphics.hpp>

namespace utilities {
	inline bool circleIntersectsRect(float cx, float cy, float radius, const sf::FloatRect& rect) {
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
		float closestX = std::clamp(cx, rect.left, rect.left + rect.width);
		float closestY = std::clamp(cy, rect.top, rect.top + rect.height);

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
}