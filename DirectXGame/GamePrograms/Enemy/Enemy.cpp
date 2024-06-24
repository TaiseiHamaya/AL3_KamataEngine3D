#include "Enemy.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG
#include "Player/Player.h"
#include "GameScene.h"
#include "Transform3D.h"
#include "BaseEnemyState.h"

Enemy::Enemy() : pPlayer(nullptr) {
	hitpoint = 10;
}

void Enemy::initialize(const std::weak_ptr<Model>& model_, const Vector3& position, uint32_t textureHandle_) {
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = position;
	model = model_;
	velocity = { 0,0,-0.5f };
	worldTransform.UpdateMatrix();
	set_state(new EnemyStateApproch);

	set_radius(2.5f);
	collisionAttribute = 0b0100;
	collisionMask = 0b0010;
}

void Enemy::update() {
	//// imgui debug
	//ImGui::SetNextWindowPos({ 50,200 }, ImGuiCond_Once);
	//ImGui::SetNextWindowSize({ 210,125 }, ImGuiCond_Once);
	//ImGui::Begin("Enemy", nullptr, ImGuiWindowFlags_NoSavedSettings);
	//ImGui::DragFloat3("Scale", &worldTransform.scale_.x, 0.1f);
	//ImGui::DragFloat3("Rotate", &worldTransform.rotation_.x, 0.02f);
	//ImGui::DragFloat3("Position", &worldTransform.translation_.x, 0.5f);
	//if (ImGui::Button("ResetPhase")) {
	//	phase = EnemyPhase::kEnemyPhaseApproach;
	//	init_approach();
	//	velocity = { 0,0,-0.5f };
	//}
	//ImGui::End();

	state->update();

	worldTransform.translation_ += velocity;
	worldTransform.UpdateMatrix();
}

void Enemy::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
}

void Enemy::on_collision() {
	--hitpoint;
}

void Enemy::fire() {
	gameScene->add_enemy_bullets(get_position());
}

bool Enemy::is_dead() const {
	return hitpoint <= 0;
}

Vector3 Enemy::get_position() const {
	return Transform3D::ExtractPosition(worldTransform.matWorld_);
}

void Enemy::set_player(Player* player) {
	pPlayer = player;
}

void Enemy::set_game_scene(GameScene* gameScene_) {
	gameScene = gameScene_;
}

void Enemy::set_velocity(Vector3&& velocity_) {
	velocity = velocity_;
}

void Enemy::set_state(BaseEnemyState* newState) {
	state = newState;
	state->set_enemy(this);
}
