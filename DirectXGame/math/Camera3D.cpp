#include "Camera3D.h"

#include <cmath>

#include <WinApp.h>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#undef near
#undef far

std::unique_ptr<Camera3D> Camera3D::instance;

void Camera3D::Initialize() {
	instance.reset(new Camera3D{});
	instance->camera = Transform3D{ Vec3::kBasis, Quaternion{ 0.5f,0,0 }, { 0, 10.0f, -20.0f } };
	instance->SetNDCInfomation(-160, 200, 300, 160, 0, 1000);
	instance->SetPerspectiveFovInfomation(0.45f, float(WinApp::kWindowWidth) / WinApp::kWindowHeight, 0.1f, 1000);
	instance->SetViewportInformation({ 0, 0 }, { WinApp::kWindowWidth, WinApp::kWindowHeight }, 0, 1);
	instance->InstanceCameraUpdate();
	instance->cameraDirection = Transform3D::HomogeneousVector(Vec3::kBasisZ, instance->camera.get_matrix());
}

void Camera3D::SetCameraPos(const Vector3& pos) {
	instance->camera.set_translate(pos);
}

void Camera3D::SetCameraTransform(const Transform3D& transform) {
	instance->camera = transform;
}

void Camera3D::SetNDCInfomation(float left, float right, float bottom, float top, float near, float far) {
	instance->ndcLeftBottomNear = { left,bottom, near };
	instance->ndcRightTopFar = { right, top, far };
}

void Camera3D::SetPerspectiveFovInfomation(float fovY, float aspectRatio, float nearClip, float farClip) {
	instance->fovY = fovY;
	instance->aspectRatio = aspectRatio;
	instance->nearClip = nearClip;
	instance->farClip = farClip;
}

void Camera3D::SetViewportInformation(const Vector2& origin, const Vector2& size, float minDepth, float maxDepth) {
	instance->viewportOrigin = origin;
	instance->viewportSize = size;
	instance->minDepth = minDepth;
	instance->maxDepth = maxDepth;
}

void Camera3D::Begin() {
	instance->camera.begin();
}

void Camera3D::CameraUpdate() {
	instance->InstanceCameraUpdate();
}

#ifdef _DEBUG
void Camera3D::DebugGUI() {
	instance->_DebugGUI();
}
#endif // _DEBUG

const Matrix4x4& Camera3D::GetOrthoMatrix() {
	return instance->orthoMatrix;
}

const Matrix4x4& Camera3D::GetPerspectiveFovMatrix() {
	return instance->perspectiveFovMatrix;
}

const Matrix4x4& Camera3D::GetViewPortMatrix() {
	return instance->viewportMatrix;
}

const Matrix4x4& Camera3D::GetVPOVMatrix() {
	return instance->vpovMatrix;
}

const Vector3& Camera3D::GetCameraDirection() {
	return instance->cameraDirection;
}

const Transform3D& Camera3D::GetCameraTransform() {
	return instance->camera;
}

void Camera3D::InstanceCameraUpdate() {
	camera.update();
	MakeViewMatrix();
	MakePersectiveFovMatrix();
	MakeOrthoMatrix();
	MakeViewportMatrix();
	vpovMatrix = viewMatrix * perspectiveFovMatrix * viewportMatrix;
	cameraDirection = Transform3D::HomogeneousVector(Vec3::kBasisZ, camera.get_matrix());
}

#ifdef _DEBUG
void Camera3D::_DebugGUI() {
	ImGui::SetNextWindowSize(ImVec2{ 300,150 }, ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2{ 50, 50 }, ImGuiCond_Once);
	ImGui::Begin("Camera", nullptr, ImGuiWindowFlags_NoSavedSettings);
	camera.debug_gui();
	ImGui::End();
}
#endif // _DEBUG

void Camera3D::MakeViewMatrix() {
	viewMatrix = camera.get_matrix().inverse();
}

void Camera3D::MakeOrthoMatrix() {
	orthoMatrix = {
		{{ 2 / (ndcRightTopFar.x - ndcLeftBottomNear.x),0,0,0},
		{0,2 / (ndcRightTopFar.y - ndcLeftBottomNear.y),0,0},
		{0,0, 1 / (ndcRightTopFar.z - ndcLeftBottomNear.z),0},
		{(ndcLeftBottomNear.x + ndcRightTopFar.x) / (ndcLeftBottomNear.x - ndcRightTopFar.x),(ndcLeftBottomNear.y + ndcRightTopFar.y) / (ndcLeftBottomNear.y - ndcRightTopFar.y), ndcLeftBottomNear.z / (ndcLeftBottomNear.z - ndcRightTopFar.z), 1}}

	};
}

void Camera3D::MakePersectiveFovMatrix() {
	float cot = 1 / std::tan(fovY / 2);
	perspectiveFovMatrix = {
		{{ cot / aspectRatio, 0, 0, 0 },
		{ 0, cot, 0, 0 },
		{ 0, 0, farClip / (farClip - nearClip), 1 },
		{ 0, 0, -nearClip * farClip / (farClip - nearClip), 0 } }
	};
}

void Camera3D::MakeViewportMatrix() {
	viewportMatrix = {
		{{ viewportSize.x / 2, 0, 0, 0 },
		{ 0, -viewportSize.y / 2, 0, 0 },
		{ 0, 0, maxDepth - minDepth, 0 },
		{ viewportOrigin.x + viewportSize.x / 2, viewportOrigin.y + viewportSize.y / 2, minDepth, 1 } }
	};
}
