#pragma once

#include <list>

#include <WorldTransform.h>
#include <Model.h>

class Input;
class Sprite;
class Reticle;
class Enemy;

class Player {
public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_, Vector3&& position);
	void update();
	void draw(const ViewProjection& viewProjection) const;

public:
	Vector3 get_position() const;
	const WorldTransform& get_transform() const;
	void on_collision();

	void set_parent(const WorldTransform* parent);

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;
	int attackTimer;

	Input* input;

	const float kCharacterSpeed = 0.5f;
	const float kCharacterRotateSpeed = 0.02f;
};