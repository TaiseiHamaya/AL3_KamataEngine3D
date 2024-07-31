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
	void draw_ui();

private: //privateメンバ関数
	void attack();
	void add_bullet(Vector3&& targetPosition);

public:
	Vector3 get_position() const;
	const WorldTransform& get_transform() const;
	void on_collision();

	void set_parent(const WorldTransform* parent);
	void set_viewprojection(const ViewProjection* const viewProjection);

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;
	int attackTimer;

	Input* input;

	std::unique_ptr<Reticle> reticle;

	const float kCharacterSpeed = 0.5f;
	const float kCharacterRotateSpeed = 0.02f;
};