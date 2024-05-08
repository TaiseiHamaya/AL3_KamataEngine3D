#include "Bullet.h"

#include <TextureManager.h>

void Bullet::initialize(std::weak_ptr<Model>& p_model, const Vector3& position) {
	model = p_model;
	transform.Initialize();
	transform.translation_ = position;
	textureHandle = TextureManager::Load("uvChecker.png");
}

void Bullet::update() {
	transform.UpdateMatrix();
}

void Bullet::draw(const ViewProjection& viewProjection) {
	model.lock()->Draw(transform, viewProjection, textureHandle);
}
