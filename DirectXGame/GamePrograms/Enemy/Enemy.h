#pragma once

#include <WorldTransform.h>
#include <Model.h>
#include <list>

#include "EnemyBullet.h"

class Player;

enum class EnemyPhase {
	kEnemyPhaseApproach,
	kEnemyPhaseWithdrawal,
};

class Enemy {
public: // コンストラクタ
	Enemy();
	Enemy(Player* const player);
	~Enemy() = default;

public: // publicメンバ関数
	void initialize(const std::weak_ptr<Model>& model_, const Vector3& position, uint32_t textureHandle_);
	void update();
	void draw(const ViewProjection& viewProjection) const;

private:
	void fire();
	void init_approach();

private: // メンバ変数
	WorldTransform worldTransform;
	std::weak_ptr<Model> model;
	uint32_t textureHandle;
	Vector3 velocity;
	EnemyPhase phase;
	std::list<EnemyBullet> bullets;
	int shotTimer;
	static constexpr int shotDistance = 60;

	Player* const pPlayer;
};
