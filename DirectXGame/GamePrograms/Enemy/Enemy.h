#pragma once

#include <WorldTransform.h>
#include <Model.h>

#include "EnemyBullet.h"

#include "Collider.h"

class Player;
class GameScene;
class BaseEnemyState;

class Enemy : public Collider {
public: // コンストラクタ
	Enemy();
	~Enemy() = default;

public: // publicメンバ関数
	void initialize(const std::weak_ptr<Model>& model_, const Vector3& position, uint32_t textureHandle_);
	void update();
	void draw(const ViewProjection& viewProjection) const;
	void fire();

public:
	bool is_dead() const;

	void on_collision() override;
	Vector3 get_position() const override;

	void set_player(Player* player);
	void set_game_scene(GameScene* gameScene_);
	void set_velocity(Vector3&& velocity_);
	void set_state(BaseEnemyState* newState);

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;
	Vector3 velocity;
	int hitpoint;

	BaseEnemyState* state;

	Player* pPlayer;
	GameScene* gameScene;
};
