#include "EnemyBullet.h"

#include <TextureManager.h>

#include "Transform3D.h"
#include "Player/Player.h"

#include "DebugText.h"

Player* EnemyBullet::player = nullptr;

void EnemyBullet::initialize(std::weak_ptr<Model> p_model, const Vector3& position, const Vector3& direction) {
	model = p_model;
	transform.Initialize();
	transform.translation_ = position;
	transform.scale_ = { 0.5f, 0.5f, 3.0f };
	textureHandle = TextureManager::Load("Red.png");
	velocity = direction * speed;

	set_direction();

	isDead = false;
	deathTimer = LifeTime;
}

void EnemyBullet::update() {
	--deathTimer;
	if (deathTimer <= 0) {
		isDead = true;
	}
	Vector3 playerPos = player->get_position() - transform.translation_;
	velocity = Vector3::Slerp(velocity, playerPos.length() ? playerPos.normalize() : Vec3::kBasisX, 0.08f);
	if (std::isnan(velocity.x)) {
		DebugText::GetInstance()->ConsolePrintf("Error");
	}
	set_direction();

	transform.translation_ += velocity;
	transform.UpdateMatrix();
}

void EnemyBullet::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(transform, viewProjection, textureHandle);
}

void EnemyBullet::on_collision() {
	isDead = true;
}

Vector3 EnemyBullet::get_position() const {
	return Transform3D::ExtractPosition(transform.matWorld_);
}

void EnemyBullet::set_direction() {
	transform.rotation_.y = std::atan2(velocity.x, velocity.z);

	float xzLength = Vector2{ velocity.x, velocity.z }.length();
	transform.rotation_.x = std::atan2(-velocity.y, xzLength);
}

void EnemyBullet::SetPlayer(Player* plyaer_) {
	player = plyaer_;
}
