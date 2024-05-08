#include "Enemy.h"

void Enemy::initialize(const std::weak_ptr<Model>& model_, const Vector3& position, uint32_t textureHandle_) {
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = position;
	model = model_;
	velocity = { 0,0,-0.5f };
}

void Enemy::update() {
	worldTransform.translation_ += velocity;
	worldTransform.UpdateMatrix();
}

void Enemy::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
}
