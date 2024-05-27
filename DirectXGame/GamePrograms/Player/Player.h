#pragma once

#include <TextureManager.h>
#include <WorldTransform.h>
#include <Model.h>
#include <list>

#include "PlayerBullet.h"

class Input;

class Player {
public: // コンストラクタ
	Player() = default;
	~Player();

public: // publicメンバ関数
	void initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_);
	void update();
	void draw(const ViewProjection& viewProjection) const;

private: //privateメンバ関数
	void attack();

public:
	Vector3 GetPosition() const;

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;

	const float kCharacterSpeed = 0.5f;
	const float kCharacterRotateSpeed = 0.02f;

	Input* input;

	std::list<PlayerBullet> bullets;
};