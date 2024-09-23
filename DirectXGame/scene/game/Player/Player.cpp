#include "Player.h"

#include "Transform3D.h"

#include "Camera3D.h"
#include "GameTimer.h"
#include "Definition.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Player::Player() = default;

Player::~Player() = default;

void Player::initialize() {
	MultiModelInstance::initialize();

	basePartsOffset[PlayerParts::Body] = { 0.0f, 0.0f , 0.0f };
	basePartsOffset[PlayerParts::Head] = { 0.0f, 1.46f, 0.0f };
	basePartsOffset[PlayerParts::ArmL] = { -0.5f, 1.26f, 0.0f };
	basePartsOffset[PlayerParts::ArmR] = { 0.5f, 1.26f, 0.0f };
}

void Player::update() {
	move();
	rotation();
	floating();
	arm_swing();
}

void Player::input(const XINPUT_STATE& joyState) {
	constexpr float DEADZONE = 0.2f;

	inputStickL.x = joyState.Gamepad.sThumbLX / float((std::numeric_limits<SHORT>::max)());
	inputStickL.y = joyState.Gamepad.sThumbLY / float((std::numeric_limits<SHORT>::max)());

	if (inputStickL.length() <= DEADZONE) {
		inputStickL = CVector2::ZERO;
	}
}

void Player::move() {
	Vector3 baseMove = { inputStickL.x, 0, inputStickL.y };
	constexpr float MOVESPEED = 7.5f;
	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	velocity = baseMove * rotation * MOVESPEED;

	transform.plus_translate(velocity * GameTimer::DeltaTime());
}

void Player::rotation() {
	if (velocity != CVector3::ZERO) {
		Quaternion afterRotation = Quaternion::LookForward(velocity.normalize());
		const Quaternion& beforeRotation = transform.get_quaternion();
		transform.set_rotate(
			Quaternion::Slerp(beforeRotation, afterRotation, 0.2f)
		);
	}
}

void Player::floating() {
	constexpr float cycle = 2.0f;
	constexpr float stepParSec = PI2 / cycle;
	floatingParameter += stepParSec * GameTimer::DeltaTime();
	floatingParameter = std::fmod(floatingParameter, PI2);
	constexpr float floatWidth = 0.3f;
	Vector3 displacement{ 0, std::sin(floatingParameter) * floatWidth, 0 };
	for (int i = 0; i < PLAYER_NUM_PARTS; ++i) {
		partsInstance[i].get_transform().set_translate(
			basePartsOffset[i] + displacement
		);
	}
}

void Player::arm_swing() {
	constexpr float cycle = 2.0f;
	constexpr float maxAngle = PI / 6;
	swingTimer += GameTimer::DeltaTime();
	swingTimer = std::fmod(swingTimer, cycle);
	float swingAngle = maxAngle * std::sin(PI2 * swingTimer / cycle);
	Quaternion swingRotation = Quaternion::AngleAxis(CVector3::BASIS_X, swingAngle);
	partsInstance[PlayerParts::ArmL].get_transform().set_rotate(swingRotation);
	partsInstance[PlayerParts::ArmR].get_transform().set_rotate(swingRotation);
}

void Player::set_camera(const Camera3D* camera_) {
	camera = camera_;
}

#ifdef _DEBUG
void Player::debug_gui() {
	ImGui::Begin("Player", nullptr);
	transform.debug_gui();
	ImGui::Separator();
	ImGui::Text("BodyOffset");
	ImGui::DragFloat3("Body", &basePartsOffset[PlayerParts::Body].x, 0.01f);
	ImGui::DragFloat3("Head", &basePartsOffset[PlayerParts::Head].x, 0.01f);
	ImGui::DragFloat3("ArmL", &basePartsOffset[PlayerParts::ArmL].x, 0.01f);
	ImGui::DragFloat3("ArmR", &basePartsOffset[PlayerParts::ArmR].x, 0.01f);
	ImGui::End();
}
#endif // _DEBUG
