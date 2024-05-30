#pragma once

#include <WorldTransform.h>
#include <Model.h>
#include <list>

#include "EnemyBullet.h"

class Player;
class GameScene;

enum class EnemyPhase {
	kEnemyPhaseApproach,
	kEnemyPhaseWithdrawal,
};

class Enemy {
public: // コンストラクタ
	Enemy();
	~Enemy() = default;

public: // publicメンバ関数
	void initialize(const std::weak_ptr<Model>& model_, const Vector3& position, uint32_t textureHandle_);
	void update();
	void draw(const ViewProjection& viewProjection) const;
	void on_collision();

private:
	void fire();
	void init_approach();

public:
	bool is_dead() const;
	Vector3 get_position() const;

	void set_player(Player* player);
	void set_game_scene(GameScene* gameScene_);

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;
	Vector3 velocity;
	EnemyPhase phase;
	int hitpoint;
	int shotTimer;
	static constexpr int shotInterval = 60;

	Player* pPlayer;
	GameScene* gameScene;
};
