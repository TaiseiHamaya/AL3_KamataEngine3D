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
	void initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_, Vector3&& position);
	void update();
	void draw(const ViewProjection& viewProjection) const;
	void on_collision();

private: //privateメンバ関数
	void attack();

public:
	Vector3 get_position() const;
	std::list<PlayerBullet>& get_bullets();
	const std::list<PlayerBullet>& get_bullets() const;

	void set_parent(const WorldTransform* parent);

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;

	const float kCharacterSpeed = 0.5f;
	const float kCharacterRotateSpeed = 0.02f;

	Input* input;

	std::list<PlayerBullet> bullets;
};