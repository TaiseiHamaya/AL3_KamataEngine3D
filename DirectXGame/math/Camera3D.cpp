#include "Camera3D.h"

#include <cmath>

#include "WinApp.h"
#undef near
#undef far

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

void Camera3D::initialize() {
	WorldInstance::initialize();

	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();

	transform = {
		CVector3::BASIS,
		Quaternion::EulerDegree(45,0,0),
		{0,10,-10}
	};

	set_perspective_fov_info(
		0.45f,
		static_cast<float>(WinApp::kWindowWidth) / static_cast<float>(WinApp::kWindowHeight),
		0.1f, 1000
	);

#ifdef _DEBUG
	debugCamera = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);
	input = Input::GetInstance();
#endif // _DEBUG
	matrix_update();
}

void Camera3D::matrix_update() {
	WorldInstance::matrix_update();
#ifdef _DEBUG
	if (isDebugCameraActive) {
		// デバッグカメラ時の更新
		// ViewProjectionをDebugCameraから更新
		auto&& debugVP = debugCamera->GetViewProjection();
		viewProjection->matView = debugVP.matView;
		viewProjection->matProjection = debugVP.matProjection;
		viewProjection->TransferMatrix();
	}
	else {
		// 通常更新
		make_view_matrix();
		make_perspectivefov_matrix();
		viewProjection->TransferMatrix();
	}
#else
	// リリース時は通常更新
	make_view_matrix();
	make_perspectivefov_matrix();
	viewProjection->TransferMatrix();
#endif
}

void Camera3D::set_transform(const Transform3D& transform_) noexcept {
	transform.copy(transform_);
}

void Camera3D::set_perspective_fov_info(float fovY_, float aspectRatio_, float nearClip_, float farClip_) noexcept {
	this->fovY = fovY_;
	this->aspectRatio = aspectRatio_;
	this->nearClip = nearClip_;
	this->farClip = farClip_;
}

const ViewProjection& Camera3D::get_view_projection() const {
	return *viewProjection;
}

#ifdef _DEBUG
void Camera3D::debug_gui() {
	transform.debug_gui();
	ImGui::Checkbox("DebugCamera", &isDebugCameraActive);
}

void Camera3D::debug_camera() {
	// デバッグカメラ
	if (input->TriggerKey(DIK_F1)) {
		isDebugCameraActive = !isDebugCameraActive;
	}
	if (isDebugCameraActive) {
		// ImGuiにMOしている際は更新しない
		if (!ImGui::GetIO().WantCaptureMouse) {
			debugCamera->Update();
		}
	}
}
#endif // _DEBUG

void Camera3D::make_view_matrix() {
	viewProjection->matView = hierarchy.matWorld_.inverse();
}

void Camera3D::make_perspectivefov_matrix() {
	float cot = 1 / std::tan(fovY / 2);
	viewProjection->matProjection = {
		{{ cot / aspectRatio, 0, 0, 0 },
		{ 0, cot, 0, 0 },
		{ 0, 0, farClip / (farClip - nearClip), 1 },
		{ 0, 0, -nearClip * farClip / (farClip - nearClip), 0 } }
	};
}

Matrix4x4 Camera3D::MakeViewportMatrix(const Vector2& origin, const Vector2& size, float minDepth, float maxDepth) {
	return {
		{{ size.x / 2, 0, 0, 0 },
		{ 0, -size.y / 2, 0, 0 },
		{ 0, 0, maxDepth - minDepth, 0 },
		{ origin.x + size.x / 2, origin.y + size.y / 2, minDepth, 1 } }
	};
}

