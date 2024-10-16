#include "Player.h"

#include "Transform3D.h"

#include "Camera3D.h"
#include "Definition.h"
#include "GameTimer.h"

#include "GlobalValues.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Player::Player() = default;

Player::~Player() = default;

void Player::initialize() {
	MultiModelInstance::initialize();

	add_global_value();
	get_global_value();

	partsInstance[PlayerParts::Hammer].set_active(false);
	partsInstance[PlayerParts::Hammer].get_transform().set_translate(basePartsOffset[PlayerParts::Hammer]);

	behavior = PlayerBehavior::Root;
	behavior_root_initialize();
	behaviorRequest = std::nullopt;
}

void Player::update() {
	behavior_update();
}

void Player::input(const XINPUT_STATE& joyState) {
	constexpr float DEADZONE = 0.2f;

	inputStickL.x = joyState.Gamepad.sThumbLX / float((std::numeric_limits<SHORT>::max)());
	inputStickL.y = joyState.Gamepad.sThumbLY / float((std::numeric_limits<SHORT>::max)());

	if (inputStickL.length() <= DEADZONE) {
		inputStickL = CVector2::ZERO;
	}

	isPressA = joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	isPressRB = joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER;
}

void Player::move() {
		auto& globalValues = GlobalValues::GetInstance();
	Vector3 baseMove = { inputStickL.x, 0, inputStickL.y };
	float MOVESPEED = globalValues.get_value<float>("Player", "MoveSpeed");
	Vector3 cameraForward = CVector3::BASIS_Z * camera->get_transform().get_quaternion();
	Vector3 xzForward = { cameraForward.x, 0, cameraForward.z };
	Quaternion rotation = Quaternion::FromToRotation(CVector3::BASIS_Z, xzForward.normalize_safe(1e-4f, CVector3::BASIS_Z));

	velocity = baseMove * rotation * MOVESPEED;

	transform.plus_translate(velocity * GameTimer::DeltaTime());
}

void Player::rotation() {
	if (velocity != CVector3::ZERO) {
		auto& globalValues = GlobalValues::GetInstance();
		Quaternion afterRotation = Quaternion::LookForward(velocity.normalize());
		const Quaternion& beforeRotation = transform.get_quaternion();
		transform.set_rotate(
			Quaternion::Slerp(beforeRotation, afterRotation, globalValues.get_value<float>("PlayerAnimation", "RotationInfluence"))
		);
	}
}

void Player::floating() {
	auto& globalValues = GlobalValues::GetInstance();
	float cycle = globalValues.get_value<float>("PlayerAnimation", "FloatingCycle");
	float stepParSec = PI2 / cycle;
	floatingParameter += stepParSec * GameTimer::DeltaTime();
	floatingParameter = std::fmod(floatingParameter, PI2);
	float floatWidth = globalValues.get_value<float>("PlayerAnimation", "FloatingWidth");
	Vector3 displacement{ 0, std::sin(floatingParameter) * floatWidth, 0 };
	for (int i = 0; i < PLAYER_NUM_PARTS; ++i) {
		partsInstance[i].get_transform().set_translate(
			basePartsOffset[i] + displacement
		);
	}
}

void Player::arm_swing() {
	auto& globalValues = GlobalValues::GetInstance();
	auto& value = std::get<WorkRoot>(behaviorValue);
	float cycle = globalValues.get_value<float>("PlayerAnimation", "SwingCycle");
	float maxAngle = globalValues.get_value<float>("PlayerAnimation", "SwingAngle");
	float swingInfluence = globalValues.get_value<float>("PlayerAnimation", "SwingInfluence");
	value.swingTimer += GameTimer::DeltaTime();
	value.swingTimer = std::fmod(value.swingTimer, cycle);
	float swingAngle = maxAngle * -std::cos(PI2 * value.swingTimer / cycle);
	Quaternion swingRotation = Quaternion::AngleAxis(CVector3::BASIS_X, swingAngle);
	const Quaternion& internal = partsInstance[PlayerParts::ArmL].get_transform().get_quaternion();
	partsInstance[PlayerParts::ArmL].get_transform().set_rotate(
		Quaternion::Slerp(internal, swingRotation, swingInfluence)
	);
	partsInstance[PlayerParts::ArmR].get_transform().set_rotate(
		Quaternion::Slerp(internal, swingRotation, swingInfluence)
	);
}

void Player::behavior_update() {
	if (behaviorRequest.has_value()) {
		switch (behaviorRequest.value()) {
		case PlayerBehavior::Root:
			behavior_root_initialize();
			break;
		case PlayerBehavior::Attack:
			behavior_attack_initialize();
			break;
		case PlayerBehavior::Dash:
			behavior_dash_initialize();
			break;
		default:
			break;
		}
		behavior = behaviorRequest.value();
		behaviorRequest = std::nullopt;
	}
	switch (behavior) {
	case Player::PlayerBehavior::Root:
		behavior_root_update();
		break;
	case Player::PlayerBehavior::Attack:
		behavior_attack_update();
		break;
	case PlayerBehavior::Dash:
		behavior_dash_update();
		break;
	default:
		break;
	}
}

void Player::behavior_root_initialize() {
	behaviorValue = WorkRoot(0.0f);
}

void Player::behavior_root_update() {
	move();
	rotation();
	floating();
	arm_swing();
	if (isPressA) {
		behaviorRequest = PlayerBehavior::Attack;
	}
	else if (isPressRB) {
		behaviorRequest = PlayerBehavior::Dash;
	}
}

void Player::behavior_attack_initialize() {
	behaviorValue = WorkAttack{ 0.0f };
	partsInstance[PlayerParts::Hammer].set_active(true);
}

void Player::behavior_attack_update() {
	auto& globalValues = GlobalValues::GetInstance();
	float ANIMATION_TIME = globalValues.get_value<float>("PlayerAnimation", "AttackTime");
	float MOVE_SPEED = globalValues.get_value<float>("PlayerAnimation", "AttackMoveSpeed");
	auto& value = std::get<WorkAttack>(behaviorValue);
	value.timer += GameTimer::DeltaTime();
	// アニメーションが終了していれば遷移させる
	if (value.timer >= ANIMATION_TIME) {
		partsInstance[PlayerParts::Hammer].set_active(false);
		behaviorRequest = PlayerBehavior::Root;
	}
	const float angleParametric = -std::sin(value.timer / ANIMATION_TIME * PI * 1.5f) * 0.5f + 0.5f;
	const float velocityParametric = value.timer / ANIMATION_TIME;
	const Quaternion internal = CQuaternion::BACK_X;
	const Quaternion terminal = Quaternion::AngleAxis(CVector3::BASIS_X, -PI / 2);
	const Quaternion rotation = Quaternion::Slerp(internal, terminal, angleParametric);
	const Vector3 forward = CVector3::BASIS_Z * transform.get_quaternion();

	transform.plus_translate(forward * velocityParametric * MOVE_SPEED * GameTimer::DeltaTime());
	partsInstance[PlayerParts::ArmL].get_transform().set_rotate(rotation);
	partsInstance[PlayerParts::ArmR].get_transform().set_rotate(rotation);
	partsInstance[PlayerParts::Hammer].get_transform().set_rotate(CQuaternion::BACK_X * rotation);
}

void Player::behavior_dash_initialize() {
	behaviorValue = WorkDash{ 0.0f };
}

void Player::behavior_dash_update() {
	auto& globalValues = GlobalValues::GetInstance();
	auto& value = std::get<WorkDash>(behaviorValue);
	value.timer += GameTimer::DeltaTime();
	float DashTime = globalValues.get_value<float>("PlayerAnimation", "DashTime");
	float DashStiffness = globalValues.get_value<float>("PlayerAnimation", "DashStiffness");
	float DashArmAngle = globalValues.get_value<float>("PlayerAnimation", "DashArmAngle");
	float DashSpeedMagnification = globalValues.get_value<float>("PlayerAnimation", "DashSpeedMagnification");
	if (value.timer >= DashTime + DashStiffness) {
		behaviorRequest = PlayerBehavior::Root;
	}
	if (value.timer <= DashTime) {
		float armAngleParametric = value.timer / DashTime;
		const Quaternion& internal = partsInstance[PlayerParts::ArmL].get_transform().get_quaternion();
		const Quaternion terminal = Quaternion::AngleAxis(CVector3::BASIS_X, DashArmAngle);
		partsInstance[PlayerParts::ArmL].get_transform().set_rotate(
			Quaternion::Slerp(internal, terminal, armAngleParametric)
		);
		partsInstance[PlayerParts::ArmR].get_transform().set_rotate(
			Quaternion::Slerp(internal, terminal, armAngleParametric)
		);
	}
	float speed = std::sin(value.timer / (DashTime + DashStiffness) * PI) * DashSpeedMagnification;
	const Vector3 forward = CVector3::BASIS_Z * transform.get_quaternion();
	transform.plus_translate(forward * speed * GameTimer::DeltaTime());
}

void Player::add_global_value() {
	auto& globalValues = GlobalValues::GetInstance();
	globalValues.add_value("PlayerParts", "Body", Vector3{ 0.0f, 0.0f , 0.0f });
	globalValues.add_value("PlayerParts", "Head", Vector3{ 0.0f, 1.46f, 0.0f });
	globalValues.add_value("PlayerParts", "ArmL", Vector3{ -0.5f, 1.26f, 0.0f });
	globalValues.add_value("PlayerParts", "ArmR", Vector3{ 0.5f, 1.26f, 0.0f });
	globalValues.add_value("PlayerParts", "Hammer", Vector3{ 0.0f, 1.0f, 0.0f });
	globalValues.add_value("Player", "MoveSpeed", 7.5f);
	globalValues.add_value("PlayerAnimation", "RotationInfluence", 0.2f);
	globalValues.add_value("PlayerAnimation", "FloatingCycle", 2.0f);
	globalValues.add_value("PlayerAnimation", "FloatingWidth", 0.3f);
	globalValues.add_value("PlayerAnimation", "SwingCycle", 2.0f);
	globalValues.add_value("PlayerAnimation", "SwingAngle", PI / 6);
	globalValues.add_value("PlayerAnimation", "SwingInfluence", 0.1f);
	globalValues.add_value("PlayerAnimation", "AttackTime", 0.5f);
	globalValues.add_value("PlayerAnimation", "AttackMoveSpeed", 0.5f);
	globalValues.add_value("PlayerAnimation", "DashTime", 0.2f);
	globalValues.add_value("PlayerAnimation", "DashStiffness", 0.3f);
	globalValues.add_value("PlayerAnimation", "DashArmAngle", PI / 3);
	globalValues.add_value("PlayerAnimation", "DashSpeedMagnification", 50.0f);
}

void Player::get_global_value() {
	auto& globalValues = GlobalValues::GetInstance();
	basePartsOffset[PlayerParts::Body] = globalValues.get_value<Vector3>("PlayerParts", "Body");
	basePartsOffset[PlayerParts::Head] = globalValues.get_value<Vector3>("PlayerParts", "Head");
	basePartsOffset[PlayerParts::ArmL] = globalValues.get_value<Vector3>("PlayerParts", "ArmL");
	basePartsOffset[PlayerParts::ArmR] = globalValues.get_value<Vector3>("PlayerParts", "ArmR");
	basePartsOffset[PlayerParts::Hammer] = globalValues.get_value<Vector3>("PlayerParts", "Hammer");
}

void Player::set_camera(const Camera3D* camera_) {
	camera = camera_;
}

#ifdef _DEBUG
void Player::debug_gui() {
	ImGui::Begin("Player", nullptr);
	transform.debug_gui();
	ImGui::Separator();
	ImGui::Text("Behavior : %d", behavior);
	ImGui::End();
}
#endif // _DEBUG
