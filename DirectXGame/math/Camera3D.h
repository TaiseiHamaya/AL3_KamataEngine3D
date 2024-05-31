#pragma once

#include "Matrix.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Transform3D.h"

#include <memory>

class Camera3D final {
	friend class Debug;
private:// シングルトンなのでprivate
	Camera3D() = default;

public:	// コピー禁止
	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

public:
	/// <summary>
	/// カメラ位置の設定
	/// </summary>
	/// <param name="cameraMatrix_"></param>
	static void SetCameraPos(const Vector3& pos);

	static void SetCameraTransform(const Transform3D& transform);

	static void SetNDCInfomation(float left, float right, float bottom, float top, float near, float far);

	static void SetPerspectiveFovInfomation(float fovY, float aspectRatio, float nearClip, float farClip);

	static void SetViewportInformation(const Vector2& origin, const Vector2& size, float minDepth, float maxDepth);

	static void Begin();

	static void CameraUpdate();

#ifdef _DEBUG
	static void DebugGUI();
#endif // _DEBUG

	static bool IsUpdatedVPMatrix();

	static bool IsUpdatedVPVMatrix();

	static bool IsUpdatedPVMatrix();

	static const Matrix4x4& GetOrthoMatrix();
	static const Matrix4x4& GetPerspectiveFovMatrix();
	static const Matrix4x4& GetViewPortMatrix();
	static const Matrix4x4& GetVPOVMatrix();

	static const Vector3& GetCameraDirection();

public:
	static const Transform3D& GetCameraTransform();

private:
	void InstanceCameraUpdate();
	void MakeViewMatrix();
	void MakeOrthoMatrix();
	void MakePersectiveFovMatrix();
	void MakeViewportMatrix();

#ifdef _DEBUG
	void _DebugGUI();
#endif // _DEBUG

private:// 一つに絞る
	static std::unique_ptr<Camera3D> instance;

private:
	Matrix4x4 viewMatrix;
	Matrix4x4 orthoMatrix;
	Matrix4x4 perspectiveFovMatrix;
	Matrix4x4 viewportMatrix;

	Matrix4x4 vpovMatrix;

	Transform3D camera;
	Vector3 cameraDirection;

	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;

	Vector3 ndcLeftBottomNear;
	Vector3 ndcRightTopFar;

	Vector2 viewportOrigin;
	Vector2 viewportSize;
	float minDepth;
	float maxDepth;
};