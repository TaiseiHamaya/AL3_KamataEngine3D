#include "RailCamera.h"

#include "imgui.h"
#include <AxisIndicator.h>
#include <Quaternion.h>
#include "Transform3D.h"

void RailCamera::initialize(Vector3&& position, Vector3&& rotate) {
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
	transform.Initialize();
	transform.translation_ = position;
	transform.rotation_ = rotate;
	viewProjection.Initialize();
}

void RailCamera::update() {
#ifdef _DEBUG
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Position", &transform.translation_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &transform.rotation_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
	
	transform.UpdateMatrix();
	viewProjection.matView = transform.matWorld_.inverse();
	viewProjection.TransferMatrix();
}

void RailCamera::update_debug(DebugCamera* debugCamera) {
	viewProjection.matView = debugCamera->GetViewProjection().matView;
	viewProjection.matProjection = debugCamera->GetViewProjection().matProjection;
	viewProjection.TransferMatrix();
}

const ViewProjection& RailCamera::get_vp() {
	return viewProjection;
}

const WorldTransform* RailCamera::get_world_transform() {
	return &transform;
}
