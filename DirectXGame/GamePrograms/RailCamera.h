#pragma once

#include <WorldTransform.h>
#include <ViewProjection.h>
#include <DebugCamera.h>

class RailCamera {
public:
	void initialize(Vector3&& position, Vector3&& rotate);
	void update();

#ifdef _DEBUG
	void update_debug(DebugCamera* debugCamera);
#endif // _DEBUG

	const ViewProjection& get_vp();
	const WorldTransform* get_world_transform();

private:
	WorldTransform transform;
	ViewProjection viewProjection;
};
