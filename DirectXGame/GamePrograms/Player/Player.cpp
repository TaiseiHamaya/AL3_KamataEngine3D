#include "Player.h"

#include <assert.h>
#include <imgui.h>
#include <Input.h>

#include <math/Transform3D.h>
#include <algorithm>

Player::~Player() {
}

void Player::initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_) {
	assert(model_);
	model = model_;
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = {};

	input = Input::GetInstance();

	bullets.clear();
}

void Player::update() {
	// imgui debug
	ImGui::Begin("Player");
	ImGui::DragFloat3("Scale", &worldTransform.scale_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform.rotation_.x, 0.02f);
	ImGui::DragFloat3("Position", &worldTransform.translation_.x, 0.5f);
	ImGui::End();
	// input
	Vector3 move;
	if (input->PushKey(DIK_LEFT)) {
		move.x -= 1;
	}
	if (input->PushKey(DIK_RIGHT)) {
		move.x += 1;
	}
	if (input->PushKey(DIK_UP)) {
		move.y += 1;
	}
	if (input->PushKey(DIK_DOWN)) {
		move.y -= 1;
	}

	// move
	if (move != Vec3::kZero) {
		worldTransform.translation_ += move.normalize() * kCharacterSpeed;
	}
	worldTransform.translation_ = Vector3::Clamp(worldTransform.translation_, Vector3{ -33.0f, -18.0f, 0.0f}, Vector3{ 33.0f, 18.0f, 0.0f });
	
	// rotate
	if (input->PushKey(DIK_A)) {
		worldTransform.rotation_.y -= kCharacterRotateSpeed;
	}
	if (input->PushKey(DIK_D)) {
		worldTransform.rotation_.y += kCharacterRotateSpeed;
	}
	
	// attak
	attack();
	
	// update
	worldTransform.matWorld_ = Transform3D::MakeAffineMatrix(worldTransform.scale_, Quaternion{ worldTransform.rotation_.x,worldTransform.rotation_.y,worldTransform.rotation_.z }, worldTransform.translation_);
	worldTransform.TransferMatrix();

	// bullet update
	for (auto bullet_itr = bullets.begin(); bullet_itr != bullets.end(); ++bullet_itr) {
		bullet_itr->update();
	}
	bullets.remove_if([](const Bullet& bullet) {
		if (bullet.is_dead()) {
			return true;
		}
		return false;
		});
}

void Player::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
	for (auto bullet_itr = bullets.begin(); bullet_itr != bullets.end(); ++bullet_itr) {
		bullet_itr->draw(viewProjection);
	}
}

void Player::attack() {
	if (input->TriggerKey(DIK_SPACE)) {
		static constexpr float kBulletSpeed = 1.0f;
		Vector3 velocity = Transform3D::HomogeneousVector({ 0,0,kBulletSpeed }, worldTransform.matWorld_);
		bullets.emplace_back();
		bullets.back().initialize(model, worldTransform.translation_, velocity);
	}
}
