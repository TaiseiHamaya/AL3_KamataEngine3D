#pragma once

#include "Vector3.h"

class Collider{
public:
	virtual void on_collision() = 0;
	virtual Vector3 get_position() const = 0;
	void set_radius(float radius_);
	float get_radius();

private:
	float radius;
};

