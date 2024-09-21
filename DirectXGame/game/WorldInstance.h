#pragma once

#define NOMINMAX

#include <memory>

#include <ViewProjection.h>
#include <WorldTransform.h>
#include <Model.h>

#include <Transform3D.h>
#include <Vector3.h>

class WorldInstance {
public:
	WorldInstance() = default;
	virtual ~WorldInstance() = default;

	// コピー禁止、ムーブ許可
	WorldInstance(const WorldInstance&) = delete;
	WorldInstance& operator=(const WorldInstance&) = delete;
	WorldInstance(WorldInstance&&) = default;
	WorldInstance& operator=(WorldInstance&&) = default;

public:
	virtual void initialize();
	virtual void update();
	virtual void matrix_update();
	virtual void late_update();
	virtual void draw() const;
	virtual void draw(const ViewProjection& viewProjection) const;

#ifdef _DEBUG
public:
	virtual void debug_gui();
#endif // _DEBUG

public:
	Vector3 world_position() const;
	const Transform3D& get_transform() const;
	Transform3D& get_transform();
	const Matrix4x4& world_matrix() const;

	void set_parent(const WorldInstance& rhs);
	void set_model(const std::shared_ptr<Model>& model_);

protected:
	Transform3D transform;
	WorldTransform hierarchy;
	std::shared_ptr<Model> model;

public:
	static void SetStaticViewProjection(const ViewProjection& viewProjection);

protected:
	inline static const ViewProjection* defaultViewProjection = nullptr;
};
