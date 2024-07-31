#pragma once

#include <WorldTransform.h>
#include <ViewProjection.h>
#include <DebugCamera.h>

#include <vector>

class RailCamera {
public:
	void initialize(Vector3&& position, Vector3&& rotate);
	void update();
	void update_camera();

#ifdef _DEBUG
	void update_debug(DebugCamera& debugCamera);
	void draw();
#endif // _DEBUG

	const ViewProjection& get_vp();
	const WorldTransform* get_world_transform();
	void set_camera_parent(const WorldTransform& worldTransform);

private:
	WorldTransform cameraTransform;
	WorldTransform railTransfrom;
	ViewProjection viewProjection;

	std::vector<Vector3> worldDrawPoints;
	std::vector<Vector3> controlPoints;

	const std::uint32_t segmentCount = 128;
	const std::int32_t totalFrame = 60 * 60;
	std::int32_t currentFrame;

	bool isMove;
};
