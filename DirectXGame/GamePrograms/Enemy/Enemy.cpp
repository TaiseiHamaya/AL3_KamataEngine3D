#include "Enemy.h"

#include "imgui.h"
#include "Player/Player.h"
#include "Transform3D.h"

Enemy::Enemy() : pPlayer(nullptr) {
}

Enemy::Enemy(Player* const player_) :
	pPlayer(player_) {
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
	// imgui debug
	ImGui::SetNextWindowPos({ 50,200 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 210,125 }, ImGuiCond_Once);
	ImGui::Begin("Enemy", nullptr, ImGuiWindowFlags_NoSavedSettings);
	ImGui::DragFloat3("Scale", &worldTransform.scale_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform.rotation_.x, 0.02f);
	ImGui::DragFloat3("Position", &worldTransform.translation_.x, 0.5f);
	if (ImGui::Button("ResetPhase")) {
		phase = EnemyPhase::kEnemyPhaseApproach;
		init_approach();
		velocity = { 0,0,-0.5f };
	}
	ImGui::End();


	switch (phase) {
	case EnemyPhase::kEnemyPhaseApproach:
		--shotTimer;
		if (shotTimer == 0) {
			fire();
		}
		if (worldTransform.translation_.z <= 0) {
			velocity = { 0, 0.1f, 0 };
			phase = EnemyPhase::kEnemyPhaseWithdrawal;
		}
		break;
	case EnemyPhase::kEnemyPhaseWithdrawal:
		break;
	default:
		break;
	}
	for (auto bulletsItr = bullets.begin(); bulletsItr != bullets.end(); ++bulletsItr) {
		bulletsItr->update();
	}
	bullets.remove_if([](const EnemyBullet& bullet) {
		if (bullet.is_dead()) {
			return true;
		}
		return false;
		});
	worldTransform.translation_ += velocity;
	worldTransform.UpdateMatrix();
}

void Enemy::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
	for (auto bulletsItr = bullets.begin(); bulletsItr != bullets.end(); ++bulletsItr) {
		bulletsItr->draw(viewProjection);
	}
}

void Enemy::on_collision() {
	--hitpoint;
}

void Enemy::fire() {
	bullets.emplace_back();
	bullets.back().initialize(model, worldTransform.translation_, (pPlayer->get_position() - Transform3D::ExtractPosition(worldTransform.matWorld_)).normalize());
	shotTimer = shotInterval;
}

void Enemy::init_approach() {
	shotTimer = shotInterval;
}

std::list<EnemyBullet>& Enemy::get_bullets() {
	return bullets;
}

const std::list<EnemyBullet>& Enemy::get_bullets() const {
	return bullets;
}

Vector3 Enemy::get_position() const {
	return Transform3D::ExtractPosition(worldTransform.matWorld_);
}
