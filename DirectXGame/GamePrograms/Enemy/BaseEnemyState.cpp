#include "BaseEnemyState.h"

#include "Enemy.h"

BaseEnemyState::BaseEnemyState() = default;

BaseEnemyState::~BaseEnemyState() = default;

void BaseEnemyState::set_enemy(Enemy* enemy_) {
	enemy = enemy_;
}

EnemyStateApproch::EnemyStateApproch() :
	shotTimer(nullptr, 0) {
}

void EnemyStateApproch::update() {
	shotTimer.update();
	if (shotTimer.is_finished()) {
		reset_call();
	}
	if (enemy->get_position().z <= 0) {
		enemy->set_velocity({ 0, 0.1f, 0 });
		enemy->set_state(new EnemyStateWithdrawal);
	}
}

void EnemyStateApproch::set_enemy(Enemy* enemy_) {
	BaseEnemyState::set_enemy(enemy_);
	reset_call();
}

void EnemyStateApproch::reset_call() {
	shotTimer = { std::bind(&Enemy::fire, enemy), SHOT_INTERVAL };
}

void EnemyStateWithdrawal::update() {
}
