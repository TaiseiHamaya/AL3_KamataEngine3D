#include "EnemyBullet.h"

#include <TextureManager.h>
#include "Transform3D.h"

void EnemyBullet::initialize(std::weak_ptr<Model>& p_model, const Vector3& position, const Vector3& direction) {
	model = p_model;
	transform.Initialize();
	transform.translation_ = position;
	textureHandle = TextureManager::Load("Red.png");
	velocity = direction * speed;
	isDead = false;
	deathTimer = LifeTime;
}

void EnemyBullet::update() {
	--deathTimer;
	if (deathTimer <= 0) {
		isDead = true;
	}
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
