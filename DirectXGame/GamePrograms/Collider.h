#pragma once

#include "Vector3.h"
#include <numeric>

class Collider {
public:
	virtual void on_collision() = 0;
	virtual Vector3 get_position() const = 0;
	void set_radius(float radius_);
	float get_radius();
	std::uint32_t get_collision_mask();
	std::uint32_t get_collision_attribute();

private:
	float radius;

protected:
	std::uint32_t collisionMask;
	std::uint32_t collisionAttribute;
};

