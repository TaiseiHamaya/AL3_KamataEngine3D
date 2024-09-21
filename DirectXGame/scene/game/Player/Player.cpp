#include "Player.h"

#include <Input.h>
#include "Transform3D.h"

#include "Camera3D.h"
#include "GameTimer.h"

#include <cassert>

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Player::Player() = default;

Player::~Player() = default;

void Player::update() {
	Vector3 baseMove = { inputStickL.x, 0, inputStickL.y };
	constexpr float MOVESPEED = 7.5f;
	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	velocity = baseMove * rotation * MOVESPEED;

	transform.plus_translate(velocity * GameTimer::DeltaTime());

	if (velocity != CVector3::ZERO) {
		transform.set_rotate(
			Quaternion::LookForward(velocity.normalize())
		);
	}
}

void Player::input(const XINPUT_STATE& joyState) {
	constexpr float DEADZONE = 0.2f;

	inputStickL = CVector2::ZERO;

	inputStickL.x += joyState.Gamepad.sThumbLX / float((std::numeric_limits<SHORT>::max)());
	inputStickL.y += joyState.Gamepad.sThumbLY / float((std::numeric_limits<SHORT>::max)());

	if (inputStickL.length() <= DEADZONE) {
		inputStickL = CVector2::ZERO;
	}
}

void Player::set_camera(const Camera3D* camera_) {
	camera = camera_;
}

#ifdef _DEBUG
void Player::debug_gui() {
	ImGui::Begin("Player", nullptr);
	transform.debug_gui();
	ImGui::End();
}
#endif // _DEBUG
