#include "CollisionManager.h"
#include "characters.h" // Include the header file for the Human class, can't include in header due to circular dependency

void CollisionManager::registerObject(const std::shared_ptr<Collidable> object) {
	collidables.push_back(object);
}

std::shared_ptr<Collidable> CollisionManager::checkCollision(const std::shared_ptr<Collidable>& obj, float offsetX, float offsetY) {
    for (auto it = collidables.begin(); it != collidables.end();) {
        // lock the weak_ptr to get a shared_ptr
        if (auto collidable = it->lock()) {
            if (collidable == obj) {
				++it; // skip self-collision
				continue; // Avoid any more processing for this object
            }

			// get() is used to get the value of the shared_ptr
			// If the object is the player, apply the offset for more accurate collision detection
            if (auto player = dynamic_cast<Human*>(obj.get())) {
                // The way this works is that we convert the players base collision box which turns Hitbox into RectangleHitbox, 
                // and since the Collidable's children override the getCollisionBox() to return their desired hitboxes, the correct intersect is called
				// i.e if collidable is a CircleHitbox, the CircleHitbox's intersect method will be called
                // if it is a TriangleBox, the TriangleHitbox's intersect method will be called and etc.
				// The player hitbox DOES return a RectangleHitbox, but we need to cast it to access its rectangle member for shifting
				// Because by default the getCollisionBox() returns a pointer to Hitbox, which does not have a rectangle member
				// We wrap the dynamic_cast in an if statement to avoid bad casts that return nullptr
                if (auto rectHb = dynamic_cast<const RectangleHitbox*>(player->getCollisionBox())) {
                    // Make a shifted copy to avoid mutating the actual hitbox
                    RectangleHitbox movedHb = *rectHb;
                    movedHb.rect.left += offsetX;
                    movedHb.rect.top += offsetY;

                    // Now test with polymorphic intersects
                    if (collidable->getCollisionBox()->intersects(&movedHb)) {
                        return collidable;
                    }
                }
                // We can't chain by else if because if the body collides then the legs (this part) will be ignored 
                if (auto legHb = dynamic_cast<const RectangleHitbox*>(player->getLegHitbox())) {
                    RectangleHitbox movedLegHb = *legHb;

                    movedLegHb.rect.left += offsetX;
                    movedLegHb.rect.top += offsetY;

                    if (collidable->getCollisionBox()->intersects(&movedLegHb)) {
                        return collidable;
                    }
                }
            } 
            if (collidable->getCollisionBox()->intersects(obj->getCollisionBox())) {
                return collidable; // return the object it collided with
            }

            ++it; // move iterator if no collision
        }
        else {
            it = collidables.erase(it); // remove expired
        }
    }

    return nullptr; // no collision found
}
