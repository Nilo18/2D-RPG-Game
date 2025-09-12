#pragma once
#include "Hitbox.h"
#include "RectangleHitbox.h"
#include "CircleIntersectsRect.h"

struct CircleHitbox : public Hitbox {
	// Initialize the values by 0 on default
	float centerX = 0.f;
	float centerY = 0.f;
	float radius = 0.f;
	CircleHitbox(float x, float y, float r) : centerX(x), centerY(y), radius(r) {}
	CircleHitbox() : centerX(0.0), centerY(0.0), radius(0.0) {} // Dummy default constuctor for the compiler
	bool intersects(const Hitbox* obj) const override {
		// If circle intersects circle
		if (auto c = dynamic_cast<const CircleHitbox*>(obj)) {
			float dx = centerX - c->centerX;
			float dy = centerY - c->centerY;
			float radiusSum = radius + c->radius;
			return (dx * dx + dy * dy) <= (radiusSum * radiusSum);
		}

		// If circle intersects rectangle
		if (auto r = dynamic_cast<const RectangleHitbox*>(obj)) {
			if (utilities::circleIntersectsRect(centerX, centerY, radius, r->getRect())) {
				return true;
			}
		}
		return false;
	}
};