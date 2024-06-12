#pragma once

class Enemy;
#include "TimedCall.h"

class BaseEnemyState {
public:
	BaseEnemyState();
	virtual ~BaseEnemyState();

public:
	virtual void update() = 0;
	virtual void set_enemy(Enemy* enemy_);

protected:
	Enemy* enemy;
};

class EnemyStateApproch : public BaseEnemyState {
public:
	EnemyStateApproch();
	~EnemyStateApproch() = default;

public:
	void update();
	void set_enemy(Enemy* enemy_) override;
	void reset_call();

private:
	TimedCall<void(void)> shotTimer;

	static constexpr int SHOT_INTERVAL = 60;
};

class EnemyStateWithdrawal : public BaseEnemyState {
public:
	EnemyStateWithdrawal() = default;
	~EnemyStateWithdrawal() = default;

public:
	void update();
};
