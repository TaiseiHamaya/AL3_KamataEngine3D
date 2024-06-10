#include "BaseEnemyState.h"

#include "Enemy.h"

BaseEnemyState::BaseEnemyState() = default;

BaseEnemyState::~BaseEnemyState() = default;

void BaseEnemyState::set_enemy(Enemy* enemy_) {
	enemy = enemy_;
}

EnemyStateApproch::EnemyStateApproch() {
	shotTimer = SHOT_INTERVAL;
}

void EnemyStateApproch::update() {
	--shotTimer;
	if (shotTimer == 0) {
		enemy->fire();
		shotTimer = SHOT_INTERVAL;
	}
	if (enemy->get_position().z <= 0) {
		enemy->set_velocity({ 0, 0.1f, 0 });
		enemy->set_state(new EnemyStateWithdrawal);
	}
}

void EnemyStateWithdrawal::update() {
}
