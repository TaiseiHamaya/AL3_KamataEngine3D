#include "Collider.h"

void Collider::set_radius(float radius_) {
	radius = radius_;
}

float Collider::get_radius() {
	return radius;
}

std::uint32_t Collider::get_collision_mask() {
	return collisionMask;
}

std::uint32_t Collider::get_collision_attribute() {
	return collisionAttribute;
}
