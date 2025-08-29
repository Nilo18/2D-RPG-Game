#pragma once
//#ifndef COLLISIONMANAGER_H
//#define COLLISIONMANAGER_H
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "utils.h"
//#include "characters.h"
//struct RectangleHitbox;

// Base class for all types of hitboxes
struct Hitbox {
	virtual ~Hitbox() = default;
	Hitbox() = default;
	virtual sf::FloatRect getRect(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) const { return {}; }
	virtual bool intersects(const Hitbox* obj) const = 0; // Note: Pure virtual functions must always be overriden
};

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

	struct TriangleHitbox : public Hitbox {
		sf::Vector2f topPoint, leftPoint, rightPoint;

		TriangleHitbox(sf::Vector2f t, sf::Vector2f l, sf::Vector2f r)
			: topPoint(t), leftPoint(l), rightPoint(r) {
		}

		bool intersects(const Hitbox* obj) const override {
			if (auto r = dynamic_cast<const RectangleHitbox*>(obj)) {
				const FloatRect& rect = r->getRect();

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

	// Abstract class for collidable objects
	class Collidable {
	protected:
		FloatRect hitboxOffset = { 0.0f, 0.0f, 0.0f, 0.0f }; // Offset for the hitbox relative to the object's position
	public:
		// If a child object is deleted through the base class pointer, the destructor must be virtual
		// Otherwise, the child class destructor won't be called, leading to resource leaks
		// If a child object isn't deleted through a base class pointer, the virtual destructor is not strictly necessary
		virtual ~Collidable() = default;
		// This sets the hitbox offset relative to the object's position and size
		virtual void setHitboxOffset(float left, float top, float width, float height) = 0;
		virtual const Hitbox* getCollisionBox(float left = 0.0f, float top = 0.0f, float width = 0.0f, float height = 0.0f) = 0; // Collision box can be of different types (e.g., sf::FloatRect, TriangleHitbox, etc.)
	};


	class CollisionManager {
	private:
		std::vector<std::weak_ptr<Collidable>> collidables; // Store weak pointers to avoid ownership issues and keep it lightweight
	public:
		CollisionManager() = default;
		// Use pointers instead of references to allow for polymorphism and dynamic allocation and better control over object lifetimes
		void registerObject(std::shared_ptr<Collidable> obj);
		// Don't return by a reference, as the object may be deleted, leading to dangling references
		// Offsets are meant for player movement prediction, to check if the player will collide with something if it moves by offsetX/Y
		std::shared_ptr<Collidable> checkCollision(const std::shared_ptr<Collidable>& obj, float offsetX = 0.0f, float offsetY = 0.0f);
		void clear(); // Clear all registered objects, technically not needed if using smart pointers, but useful for resetting state
	};


	//#endif

