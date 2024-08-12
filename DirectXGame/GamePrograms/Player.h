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
	void initialize(const std::shared_ptr<Model>& model_, Vector3&& position);
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
	int attackTimer;

	Input* input;
};