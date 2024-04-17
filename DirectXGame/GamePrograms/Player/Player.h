#pragma once

#include <TextureManager.h>
#include <WorldTransform.h>
#include <Model.h>

class Player {
public: // コンストラクタ
	Player() = default;
	~Player() = default;

public: // publicメンバ関数
	void initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_);
	void update();
	void draw(const ViewProjection& viewProjection) const;

private: //privateメンバ関数


private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;
};