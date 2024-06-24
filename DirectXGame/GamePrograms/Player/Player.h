#pragma once

#include <list>

#include <WorldTransform.h>
#include <Model.h>

#include "PlayerBullet.h"
#include "Collider.h"

class Input;
class Sprite;

class Player : public Collider {
public: // コンストラクタ
	Player() = default;
	~Player();

public: // publicメンバ関数
	void initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_, uint32_t reticleSpriteHandle, Vector3&& position);
	void update();
	void draw(const ViewProjection& viewProjection) const;
	void draw_ui();

private: //privateメンバ関数
	void attack();

public:
	Vector3 get_position() const override;
	void on_collision() override;

	std::list<PlayerBullet>& get_bullets();
	const std::list<PlayerBullet>& get_bullets() const;

	void set_parent(const WorldTransform* parent);
	void set_viewprojection(const ViewProjection* const viewProjection);

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;

	WorldTransform transform3DReticle;
	std::unique_ptr<Sprite> sprite;
	Vector2 reticlePos;

	Input* input;

	std::list<PlayerBullet> bullets;

	const float kCharacterSpeed = 0.5f;
	const float kCharacterRotateSpeed = 0.02f;
	const ViewProjection* refViewProjection;
};