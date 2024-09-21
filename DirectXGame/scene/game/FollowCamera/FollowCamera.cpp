#include "FollowCamera.h"

#include <algorithm>

#include "Definition.h"

void FollowCamera::initialize() {
	Camera3D::initialize();

	offset = { 0,0,30 };
}

void FollowCamera::update() {
	// 入力から回転に変換
	Vector2 rotateAngle = inputStickR * ToRadian * 1.5f;

	rotateAngle.y *= -1;
	const Quaternion& rotation = transform.get_quaternion();
	// 平行成分と垂直成分でQuaternionを生成
	Quaternion holizontal = Quaternion::AngleAxis(CVector3::BASIS_Y, rotateAngle.x);
	Quaternion vertical = Quaternion::AngleAxis(CVector3::BASIS_X, rotateAngle.y);

	Vector3 beforeForward = CVector3::BASIS_Z * rotation;
	float forwardDot = Vector3::DotProduct(beforeForward, -CVector3::BASIS_Y);
	if (forwardDot >= 0.999f && rotateAngle.y > 0) {
		// 真下と視線のの差を出す。
		float angle = std::acos(std::clamp(forwardDot, -1.0f, 1.0f));
		if (angle >= 0.1f * ToRadian) {
			vertical = Quaternion::AngleAxis(CVector3::BASIS_X, angle);
		}
		else {
			vertical = CQuaternion::IDENTITY;
		}
	}
	else if (forwardDot <= 5.1f * ToRadian && rotateAngle.y < 0) {
		Vector3 beforeUpward = CVector3::BASIS_Y * rotation;
		float upwardDot = Vector3::DotProduct(beforeUpward, CVector3::BASIS_Y);
		float angle = std::acos(std::clamp(upwardDot, -1.0f, 1.0f));
		// 水平より5度下を向かせる
		if (angle >= 5.01f * ToRadian) {
			vertical = Quaternion::AngleAxis(CVector3::BASIS_X, -(angle - 5 * ToRadian));
		}
		// 変化する角度があまりに小さい場合や範囲を超える場合は回転させない
		else {
			vertical = CQuaternion::IDENTITY;
		}
	}
	// 垂直->元->平行で適用させる
	transform.set_rotate(holizontal * rotation * vertical);

	// ターゲットが設定されていない場合は
	if (!target) {
		return;
	}
	// offsetを回転させて視線を向ける
	Vector3 translate = target->world_position() - offset * transform.get_quaternion();
	transform.set_translate(translate);
}

void FollowCamera::input(const XINPUT_STATE& joyState) {
	constexpr float DEADZONE = 0.2f;

	inputStickR.x = joyState.Gamepad.sThumbRX / float((std::numeric_limits<SHORT>::max)());
	inputStickR.y = joyState.Gamepad.sThumbRY / float((std::numeric_limits<SHORT>::max)());

	if (inputStickR.length() <= DEADZONE) {
		inputStickR = CVector2::ZERO;
	}

}

void FollowCamera::set_offset(const Vector3& offset_) {
	offset = offset_;
}

const Vector3& FollowCamera::get_offset() const {
	return offset;
}

void FollowCamera::set_target(const WorldInstance* target_) {
	target = target_;
}

#ifdef _DEBUG

#include <imgui.h>
#include "FollowCamera.h"

void FollowCamera::debug_gui() {
	ImGui::Begin("Camera3D");
	ImGui::DragFloat3("Offset", &offset.x, 0.1f);
	Camera3D::debug_gui();
	ImGui::End();
}
#endif // _DEBUG
