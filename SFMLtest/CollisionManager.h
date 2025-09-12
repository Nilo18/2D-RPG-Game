#pragma once
//#ifndef COLLISIONMANAGER_H
//#define COLLISIONMANAGER_H
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "utils.h"
#include "CircleHitbox.h"
#include "TriangleHitbox.h"
//#include "characters.h"
//struct RectangleHitbox;

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

