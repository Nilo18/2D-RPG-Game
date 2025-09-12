#pragma once
#include <SFML/Graphics.hpp>

struct Hitbox {
	virtual ~Hitbox() = default;
	Hitbox() = default;
	virtual sf::FloatRect getRect(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) const { return {}; }
	virtual bool intersects(const Hitbox* obj) const = 0; // Note: Pure virtual functions must always be overriden
};