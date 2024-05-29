#include "Player.h"

#include <cassert>
#include <imgui.h>
#include <Input.h>

#include <math/Transform3D.h>

Player::~Player() {
}

void Player::initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_, Vector3&& position) {
	assert(model_);
	model = model_;
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = position;

	input = Input::GetInstance();

	bullets.clear();
}

void Player::update() {
	// imgui debug
	ImGui::SetNextWindowPos({ 50,50 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 210,100 }, ImGuiCond_Once);
	ImGui::Begin("Player", nullptr, ImGuiWindowFlags_NoSavedSettings);
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
	worldTransform.translation_ = Vector3::Clamp(worldTransform.translation_, Vector3{ -33.0f, -18.0f, 0.0f}, Vector3{ 33.0f, 18.0f, 10000.0f });
	
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
	worldTransform.UpdateMatrix();

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
	for (auto bullet_itr = bullets.begin(); bullet_itr != bullets.end(); ++bullet_itr) {
		bullet_itr->draw(viewProjection);
	}
}

void Player::on_collision() {
	// do nothing
}

void Player::attack() {
	if (input->TriggerKey(DIK_SPACE)) {
		static constexpr float kBulletSpeed = 1.0f;
		Vector3 velocity = Transform3D::HomogeneousVector({ 0,0,kBulletSpeed }, worldTransform.matWorld_);
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
