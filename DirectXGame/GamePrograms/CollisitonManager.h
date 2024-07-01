#pragma once

#include <list>

#include "Collider.h"

class CollisitonManager {
public:
	CollisitonManager() = default;
	~CollisitonManager() = default;

public:
	void reset();

	void collision() const;

	void add_collider(Collider* newCollider);

private:
	std::list<Collider*> colliderObjects;
};
