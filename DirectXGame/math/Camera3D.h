#pragma once

#include <WorldInstance.h>

#include <Input.h>
#include <DebugCamera.h>

#include <memory>

class Camera3D : public WorldInstance {
public:
	Camera3D() = default;
	virtual ~Camera3D() = default;

	Camera3D(const Camera3D&) = delete;
	Camera3D& operator=(const Camera3D&) = delete;

public:
	virtual void initialize() override;

	void matrix_update() override;

	void set_transform(const Transform3D& transform) noexcept;

	void set_perspective_fov_info(float fovY, float aspectRatio, float nearClip, float farClip) noexcept;

	const ViewProjection& get_view_projection() const;

#ifdef _DEBUG
	virtual void debug_gui();
	void debug_camera();
#endif // _DEBUG

private:
	void make_view_matrix();
	void make_perspectivefov_matrix();

public:
	static Matrix4x4 MakeViewportMatrix(const Vector2& origin, const Vector2& size, float minDepth, float maxDepth);

protected:
	std::unique_ptr<ViewProjection> viewProjection;

private:
	float fovY;
	float aspectRatio;
	float nearClip;
	float farClip;

#ifdef _DEBUG
	Input* input;
	bool isDebugCameraActive{ false };
	std::unique_ptr<DebugCamera> debugCamera;
#endif // _DEBUG
};