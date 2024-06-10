#include "Enemy.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG
#include "Player/Player.h"
#include "GameScene.h"
#include "Transform3D.h"

std::vector<void(Enemy::*)(void)> Enemy::FUNC_TABLE{
	&Enemy::approach,
	&Enemy::withdrawal
};

Enemy::Enemy() : pPlayer(nullptr) {
	hitpoint = 10;
}

void Enemy::initialize(const std::weak_ptr<Model>& model_, const Vector3& position, uint32_t textureHandle_) {
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = position;
	model = model_;
	velocity = { 0,0,-0.5f };
	phase = EnemyPhase::kEnemyPhaseApproach;
	init_approach();
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

	(this->*FUNC_TABLE[static_cast<std::uint32_t>(phase)])();

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
	shotTimer = SHOT_INTERVAL;
}

void Enemy::init_approach() {
	shotTimer = SHOT_INTERVAL;
}

void Enemy::approach() {
	--shotTimer;
	if (shotTimer == 0) {
		fire();
	}
	if (worldTransform.translation_.z <= 0) {
		velocity = { 0, 0.1f, 0 };
		phase = EnemyPhase::kEnemyPhaseWithdrawal;
	}
}

void Enemy::withdrawal() {
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
