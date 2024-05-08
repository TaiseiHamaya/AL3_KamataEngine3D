#pragma once

#include <TextureManager.h>
#include <WorldTransform.h>
#include <Model.h>

class Enemy {
public: // コンストラクタ
	Enemy() = default;
	~Enemy() = default;

public: // publicメンバ関数
	void initialize(const std::weak_ptr<Model>& model_, const Vector3& position, uint32_t textureHandle_);
	void update();
	void draw(const ViewProjection& viewProjection) const;

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;
	Vector3 velocity;
};

