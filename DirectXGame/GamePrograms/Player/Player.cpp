#include "Player.h"

#include "Camera3D.h"
#include <cassert>
#include <imgui.h>
#include <Input.h>
#include <Sprite.h>
#include <ViewProjection.h>
#include <WinApp.h>

#include <math/Transform3D.h>
#include <windef.h>

Player::~Player() = default;

void Player::initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_, uint32_t reticleSpriteHandle, Vector3&& position) {
	assert(model_);
	model = model_;
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = position;

	sprite.reset(Sprite::Create(reticleSpriteHandle, { 0,0 }, { 1,1,1,1 }, { 0.5f, 0.5f }));
	reticlePos = { static_cast<float>(WinApp::kWindowWidth / 2),static_cast<float>(WinApp::kWindowHeight / 2) };

	transform3DReticle.Initialize();

	input = Input::GetInstance();

	bullets.clear();
}

void Player::update() {
	// imgui debug
	ImGui::SetNextWindowPos({ 50,250 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 210,100 }, ImGuiCond_Once);
	ImGui::Begin("Player", nullptr, ImGuiWindowFlags_NoSavedSettings);
	ImGui::DragFloat3("Scale", &worldTransform.scale_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform.rotation_.x, 0.02f);
	ImGui::DragFloat3("Position", &worldTransform.translation_.x, 0.5f);
	ImGui::End();
	// input
	Vector3 moveStickL;
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState)) {
		moveStickL.x += float(joyState.Gamepad.sThumbLX) / (std::numeric_limits<short>::max)();
		moveStickL.y += float(joyState.Gamepad.sThumbLY) / (std::numeric_limits<short>::max)();
	}
	//if (input->PushKey(DIK_LEFT)) {
	//	moveStickL.x -= 1;
	//}
	//if (input->PushKey(DIK_RIGHT)) {
	//	moveStickL.x += 1;
	//}
	//if (input->PushKey(DIK_UP)) {
	//	moveStickL.y += 1;
	//}
	//if (input->PushKey(DIK_DOWN)) {
	//	moveStickL.y -= 1;
	//}

	// moveStickL
	if (moveStickL != Vec3::kZero) {
		worldTransform.translation_ += moveStickL.normalize() * kCharacterSpeed;
	}
	worldTransform.translation_ = Vector3::Clamp(worldTransform.translation_, Vector3{ -33.0f, -18.0f, -10000.0f }, Vector3{ 33.0f, 18.0f, 10000.0f });

	// rotate
	if (input->PushKey(DIK_A)) {
		worldTransform.rotation_.y -= kCharacterRotateSpeed;
	}
	if (input->PushKey(DIK_D)) {
		worldTransform.rotation_.y += kCharacterRotateSpeed;
	}

	// update
	worldTransform.UpdateMatrix();

	//{
	//	constexpr float ReticleDistancce = 40.0f;
	//	Vector3 offset = Transform3D::Homogeneous(Vec3::kBasisZ * ReticleDistancce, worldTransform.matWorld_);
	//	transform3DReticle.translation_ = offset;

	//	transform3DReticle.UpdateMatrix();

	//	Vector3 reticlePosition = Transform3D::Homogeneous(
	//		Transform3D::ExtractPosition(transform3DReticle.matWorld_),
	//		refViewProjection->matView * refViewProjection->matProjection * Camera3D::GetViewPortMatrix());

	//	sprite->SetPosition({reticlePosition.x - 64, reticlePosition.y - 64 });
	//}

	{
		//POINT mousePosition;
		//GetCursorPos(&mousePosition);
		//ScreenToClient(WinApp::GetInstance()->GetHwnd(), &mousePosition);
		Vector2 moveStickR{};
		if (input->GetJoystickState(0, joyState)) {
			moveStickR.x += float(joyState.Gamepad.sThumbRX) / (std::numeric_limits<short>::max)();
			moveStickR.y -= float(joyState.Gamepad.sThumbRY) / (std::numeric_limits<short>::max)();
		}
		static constexpr float kReticleMoveSpeed = 8;
		reticlePos += moveStickR * kReticleMoveSpeed;
		sprite->SetPosition(reticlePos - Vector2{ 64.0f, 64.0f });

		Matrix4x4 matrix = (refViewProjection->matView * refViewProjection->matProjection * Camera3D::GetViewPortMatrix()).inverse();

		Vector3 posNear = Transform3D::Homogeneous({ reticlePos.x, reticlePos.y, 0 }, matrix);
		Vector3 posFar = Transform3D::Homogeneous({ reticlePos.x, reticlePos.y, 1 }, matrix);

		static constexpr float kReticleDistance = 100.0f;
		Vector3 mouseDirection = (posFar - posNear).normalize() * kReticleDistance;

		transform3DReticle.translation_ = posNear + mouseDirection;

		transform3DReticle.UpdateMatrix();
	}

	// attak
	attack();

	// bullet update
	for (auto bullet_itr = bullets.begin(); bullet_itr != bullets.end(); ++bullet_itr) {
		bullet_itr->update();
	}
	bullets.remove_if([](const PlayerBullet& bullet) {
		if (bullet.is_dead()) {
			return true;
		}
		return false;
		});
}

void Player::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
	//model.lock()->Draw(transform3DReticle, viewProjection, textureHandle);
	for (auto bullet_itr = bullets.begin(); bullet_itr != bullets.end(); ++bullet_itr) {
		bullet_itr->draw(viewProjection);
	}
}

void Player::draw_ui() {
	sprite->Draw();
}

void Player::on_collision() {
	// do nothing
}

void Player::attack() {
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState) && (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		static constexpr float kBulletSpeed = 3.0f;
		Vector3 velocity = (Transform3D::ExtractPosition(transform3DReticle.matWorld_) - get_position()).normalize() * kBulletSpeed;
		bullets.emplace_back();
		bullets.back().initialize(model, get_position(), velocity);
	}
}

Vector3 Player::get_position() const {
	return Transform3D::ExtractPosition(worldTransform.matWorld_);
}

std::list<PlayerBullet>& Player::get_bullets() {
	return bullets;
}

const std::list<PlayerBullet>& Player::get_bullets() const {
	return bullets;
}

void Player::set_parent(const WorldTransform* parent) {
	worldTransform.parent_ = parent;
}

void Player::set_viewprojection(const ViewProjection* const viewProjection) {
	refViewProjection = viewProjection;
}
