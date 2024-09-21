#include "WorldInstance.h"

void WorldInstance::initialize() {
	hierarchy.Initialize();
	model.reset();
}

void WorldInstance::update() {
}

void WorldInstance::matrix_update() {
	hierarchy.UpdateMatrix(transform);
}

void WorldInstance::late_update() {
}

void WorldInstance::draw() const {
	draw(*defaultViewProjection);
}

void WorldInstance::draw(const ViewProjection& viewProjection) const {
	if (model) {
		model->Draw(hierarchy, viewProjection);
	}
}

void WorldInstance::debug_gui() {
}

Vector3 WorldInstance::world_position() const {
	return Transform3D::ExtractPosition(hierarchy.matWorld_);
}

const Transform3D& WorldInstance::get_transform() const {
	return transform;
}

Transform3D& WorldInstance::get_transform() {
	return transform;
}

const Matrix4x4& WorldInstance::world_matrix() const {
	return hierarchy.matWorld_;
}

void WorldInstance::set_parent(const WorldInstance& rhs) {
	hierarchy.parent_ = &rhs.hierarchy;
}

void WorldInstance::set_model(const std::shared_ptr<Model>& model_) {
	model = model_;
}

void WorldInstance::SetStaticViewProjection(const ViewProjection& viewProjection) {
	defaultViewProjection = &viewProjection;
}
