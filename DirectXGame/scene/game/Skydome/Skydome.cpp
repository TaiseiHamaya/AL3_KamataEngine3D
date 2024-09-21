#include "Skydome.h"

void Skydome::initialize() {
	WorldInstance::initialize();
	transform.set_scale(
		{ 100,100,100 }
	);
}
