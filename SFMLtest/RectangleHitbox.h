#pragma once
#include <SFML/Graphics.hpp>
#include "Hitbox.h"

struct RectangleHitbox : public Hitbox {
	sf::FloatRect rect;
	RectangleHitbox(float left, float top, float width, float height) : rect(left, top, width, height) {}
	RectangleHitbox() : rect(0.0, 0.0, 0.0, 0.0) {}
	RectangleHitbox& operator=(const RectangleHitbox& other) {
		this->rect.top = other.rect.top;
		this->rect.left = other.rect.left;
		this->rect.width = other.rect.width;
		this->rect.height = other.rect.height;
		return *this;
	}
	sf::FloatRect getRect(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) const override { return rect; }
	// Rectangle-Rectangle intersection
	bool intersects(const Hitbox* obj) const override {
		if (auto r = dynamic_cast<const RectangleHitbox*>(obj)) {
			return rect.intersects(r->rect); // Check if the local rect hitbox intersects the given rec hitbox
		}

		return false;
	}

	// This sets the rectangle's position and size
	void set(float left, float top, float width, float height) {
		rect = { left, top, width, height };
	}
};