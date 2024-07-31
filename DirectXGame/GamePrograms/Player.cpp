#include "Player.h"

#include <cassert>
#include <Input.h>
#include <ViewProjection.h>
#include "Transform3D.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Player::Player() = default;

Player::~Player() = default;

void Player::initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_, Vector3&& position) {
	assert(model_);
	model = model_;
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = position;

	input = Input::GetInstance();
}

void Player::update() {
	// imgui debug
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 50,250 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 210,100 }, ImGuiCond_Once);
	ImGui::Begin("Player", nullptr, ImGuiWindowFlags_NoSavedSettings);
	ImGui::DragFloat3("Scale", &worldTransform.scale_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &worldTransform.rotation_.x, 0.02f);
	ImGui::DragFloat3("Position", &worldTransform.translation_.x, 0.01f);
	ImGui::End();
#endif // _DEBUG
	// update
	worldTransform.UpdateMatrix();
}

void Player::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
}

void Player::on_collision() {
	// do nothing
}

Vector3 Player::get_position() const {
	return Transform3D::ExtractPosition(worldTransform.matWorld_);
}

const WorldTransform& Player::get_transform() const {
	return worldTransform;
}

void Player::set_parent(const WorldTransform* parent) {
	worldTransform.parent_ = parent;
}
