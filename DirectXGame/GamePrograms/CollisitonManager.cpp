#include "CollisitonManager.h"

#include "PlayerBullet.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyBullet.h"

void IsCollision(Collider* const collider1, Collider* const collider2) {
	if (!((collider1->get_collision_attribute() & collider2->get_collision_mask()) ||
		(collider2->get_collision_attribute() & collider1->get_collision_mask()))) {
		return;
	}
	if ((collider1->get_position() - collider2->get_position()).length() < collider1->get_radius() + collider2->get_radius()) {
		collider1->on_collision();
		collider2->on_collision();
	}
}

void CollisitonManager::reset() {
	colliderObjects.clear();
}

void CollisitonManager::collision() const {
	for (auto colliderItrA = colliderObjects.begin(); colliderItrA != std::prev(colliderObjects.end()); ++colliderItrA) {
		for (auto colliderItrB = std::next(colliderItrA); colliderItrB != colliderObjects.end(); ++colliderItrB) {
			IsCollision(*colliderItrA, *colliderItrB);
		}
	}
}

void CollisitonManager::add_collider(Collider* newCollider) {
	colliderObjects.emplace_back(newCollider);
}
