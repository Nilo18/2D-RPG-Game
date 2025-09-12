#pragma once
#include <SFML/Graphics.hpp>
#include "RectangleHitbox.h"
#include "PointsInRect.h"
#include "PointsInTriangle.h"

struct TriangleHitbox : public Hitbox {
	sf::Vector2f topPoint, leftPoint, rightPoint;

	TriangleHitbox(sf::Vector2f t, sf::Vector2f l, sf::Vector2f r)
		: topPoint(t), leftPoint(l), rightPoint(r) {
	}

	bool intersects(const Hitbox* obj) const override {
		if (auto r = dynamic_cast<const RectangleHitbox*>(obj)) {
			const sf::FloatRect& rect = r->getRect();

			// Check if any of the triangle's points are inside the rectangle
			if (utilities::pointInRect(topPoint, rect) ||
				utilities::pointInRect(leftPoint, rect) ||
				utilities::pointInRect(rightPoint, rect))
				return true;

			sf::Vector2f rectCorners[4] = {
				{rect.left, rect.top},
				{rect.left + rect.width, rect.top},
				{rect.left, rect.top + rect.height},
				{rect.left + rect.width, rect.top + rect.height}
			};

			// Check if any of the rectangle's corners are inside the triangle
			for (const auto& corner : rectCorners) {
				if (utilities::pointInTriangle(corner, topPoint, leftPoint, rightPoint)) {
					return true;
				}
			}

			return false;
		}

		return false;
	} // <-- close intersects
}; // <-- close TriangleHitbox here