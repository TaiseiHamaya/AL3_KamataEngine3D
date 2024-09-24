#pragma once

#include <Camera3D.h>

#include <Vector2.h>

class FollowCamera final : public Camera3D {
public:
	void initialize() override;
	void update() override;
	void matrix_update() override;

	void input(const XINPUT_STATE& joyState);

public:
	void set_offset(const Vector3& offset_);
	const Vector3& get_offset() const;

	void set_target(const WorldInstance* target_);

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private:
	Vector3 offset;

	bool isPressX;
	Vector2 inputStickR;

	const WorldInstance* target;
	WorldInstance lookAtInstance;
	Quaternion destingRotation;
};

