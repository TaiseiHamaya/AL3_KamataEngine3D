#include "RailCamera.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

#include <AxisIndicator.h>

#include "math/Definition.h"
#include "PrimitiveDrawer.h"

void RailCamera::initialize(Vector3&& position, Vector3&& rotate) {
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
	transform.Initialize();
	transform.translation_ = position;
	transform.rotation_ = rotate;
	viewProjection.Initialize();

	std::vector<Vector3> controlPoints;
	controlPoints = {
		{0, 0, 0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0, 0},
		{30, 0, 0}
	};

	const uint32_t segmentCount = 128;

	for (int i = 0; i < segmentCount; ++i) {
		worldDrawPoints.emplace_back(CatmullRom(controlPoints, 1.0f / segmentCount * i));
	}
}

void RailCamera::update() {
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 50,50 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 210,80 }, ImGuiCond_Once);
	ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	ImGui::DragFloat3("Position", &transform.translation_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &transform.rotation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG

	//transform.translation_.z += 0.1f;
	transform.UpdateMatrix();
	viewProjection.matView = transform.matWorld_.inverse();
	viewProjection.TransferMatrix();
}

#ifdef _DEBUG
void RailCamera::update_debug(DebugCamera* debugCamera) {
	viewProjection.matView = debugCamera->GetViewProjection().matView;
	viewProjection.matProjection = debugCamera->GetViewProjection().matProjection;
	viewProjection.TransferMatrix();
}

void RailCamera::draw() {
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
	for (int i = 0; i < worldDrawPoints.size() - 1; ++i) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			worldDrawPoints[i],
			worldDrawPoints[i + 1],
			{ 1.0f, 0.0f, 0.0f, 1.0f });
	}
}
#endif // _DEBUG

const ViewProjection& RailCamera::get_vp() {
	return viewProjection;
}

const WorldTransform* RailCamera::get_world_transform() {
	return &transform;
}
