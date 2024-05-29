#pragma once

#include <WorldTransform.h>
#include <Model.h>
#include <ViewProjection.h>

class Skydome {
public:
	Skydome() = default;
	~Skydome() = default;
	void initialize(std::weak_ptr<Model> model_);
	void update();
	void draw(const ViewProjection& viewProjection) const;

private:
	WorldTransform transform;
	std::weak_ptr<Model> model;
};

