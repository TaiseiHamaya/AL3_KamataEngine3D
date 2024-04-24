#include "Player.h"

#include <assert.h>
#include <imgui.h>
#include <Input.h>

#include <math/Transform3D.h>
#include <algorithm>

void Player::initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_) {
	assert(model_);
	model = model_;
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = {};

	input = Input::GetInstance();
}

void Player::update() {
	ImGui::Begin("Player");
	ImGui::DragFloat3("Position", &worldTransform.translation_.x, 0.5f);
	ImGui::End();
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
	if (move != Vec3::kZero) {
		worldTransform.translation_ += move.normalize() * kCharacterSpeed;
	}
	worldTransform.translation_ = Vector3::Clamp(worldTransform.translation_, Vector3{ -33.0f, -18.0f, 0.0f}, Vector3{ 33.0f, 18.0f, 0.0f });
	worldTransform.matWorld_ = Transform3D::MakeAffineMatrix(worldTransform.scale_, Quaternion{ worldTransform.rotation_.x,worldTransform.rotation_.y,worldTransform.rotation_.z }, worldTransform.translation_);
	worldTransform.TransferMatrix();
}

void Player::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
}
