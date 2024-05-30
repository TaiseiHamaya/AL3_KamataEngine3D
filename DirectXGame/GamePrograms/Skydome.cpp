#include "Skydome.h"

void Skydome::initialize(std::weak_ptr<Model> model_) {
	transform.Initialize();
	model = model_;
	transform.scale_ = { 500,500,500 };
}

void Skydome::update() {
	transform.UpdateMatrix();
}

void Skydome::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(transform, viewProjection);
}