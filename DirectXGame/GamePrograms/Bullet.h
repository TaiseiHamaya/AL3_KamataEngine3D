#pragma once

#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <memory>

class Bullet {
public:
	void initialize(std::weak_ptr<Model>& p_model, const Vector3& position);
	void update();
	void draw(const ViewProjection& viewProjection);

private:
	WorldTransform transform;
	std::weak_ptr<Model> model;
	std::uint32_t textureHandle;
};

