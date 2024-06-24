#include "PlayerBullet.h"

#include <TextureManager.h>
#include "Transform3D.h"

void PlayerBullet::initialize(std::weak_ptr<Model>& p_model, const Vector3& position, const Vector3& velocity_) {
	model = p_model;
	transform.Initialize();
	transform.translation_ = position;
	textureHandle = TextureManager::Load("Blue.png");
	velocity = velocity_;
	isDead = false;
	deathTimer = LifeTime;

	set_radius(2.5f);
	collisionAttribute = 0b0010;
	collisionMask = 0b1100;
}

void PlayerBullet::update() {
	--deathTimer;
	if (deathTimer <= 0) {
		isDead = true;
	}
	transform.translation_ += velocity;
	transform.UpdateMatrix();
}

void PlayerBullet::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(transform, viewProjection, textureHandle);
}

void PlayerBullet::on_collision() {
	isDead = true;
}

Vector3 PlayerBullet::get_position() const {
	return Transform3D::ExtractPosition(transform.matWorld_);
}
