#include "Player.h"

#include <assert.h>
#include <imgui.h>

void Player::initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_) {
	assert(model_);
	model = model_;
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = {};
}

void Player::update() {
	worldTransform.TransferMatrix();
}

void Player::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
	
}
