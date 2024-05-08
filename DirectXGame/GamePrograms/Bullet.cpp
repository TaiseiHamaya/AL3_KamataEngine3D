#include "Bullet.h"

#include <TextureManager.h>

void Bullet::initialize(std::weak_ptr<Model>& p_model, const Vector3& position, const Vector3& velocity_) {
	model = p_model;
	transform.Initialize();
	transform.translation_ = position;
	textureHandle = TextureManager::Load("uvChecker.png");
	velocity = velocity_;
	isDead = false;
	deathTimer = LifeTime;
}

void Bullet::update() {
	--deathTimer;
	if (deathTimer <= 0) {
		isDead = true;
	}
	transform.translation_ += velocity;
	transform.UpdateMatrix();
}

void Bullet::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(transform, viewProjection, textureHandle);
}
