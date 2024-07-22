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
	transform.scale_ = Vec3::kBasis;
	viewProjection.Initialize();
	controlPoints = {
		{0, 0, 0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0, 0},
		{30, 0, 0}
	};

	for (std::uint32_t i = 0; i < segmentCount; ++i) {
		worldDrawPoints.emplace_back(CatmullRom(controlPoints, 1.0f / segmentCount * i));
	}

	currentFrame = 0;
	isMove = true;
}

void RailCamera::update() {
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 50, 50 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 210, 150 }, ImGuiCond_Once);
	ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	ImGui::DragFloat3("Position", &transform.translation_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &transform.rotation_.x, 0.1f);
	ImGui::DragFloat3("Scale", &transform.scale_.x, 0.1f);
	ImGui::Separator();
	ImGui::Checkbox("Play", &isMove);
	ImGui::DragInt("RailCurrent", &currentFrame, 1.0f, 0, totalFrame);
	ImGui::End();
#endif // _DEBUG

	if (isMove && currentFrame < totalFrame - 1) {
		++currentFrame;
	}

	transform.translation_ = CatmullRom(controlPoints, 1.0f / totalFrame * currentFrame);

	Vector3 lookAt = (CatmullRom(controlPoints, 1.0f / totalFrame * (currentFrame + 1)) - transform.translation_);

	transform.rotation_.y = std::atan2(lookAt.x, lookAt.z);

	float xzLength = Vector2{ lookAt.x, lookAt.z }.length();
	transform.rotation_.x = std::atan2(-lookAt.y, xzLength);

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
