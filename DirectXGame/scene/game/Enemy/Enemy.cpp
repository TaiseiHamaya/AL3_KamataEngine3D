#include "Enemy.h"

#include <Definition.h>
#include <GameTimer.h>

void Enemy::initialize() {
	MultiModelInstance<ENEMY_NUM_PARTS>::initialize();
	basePartsOffset = {
		Vector3{0.0f,0.0f,0.0f},
		Vector3{1.0f,1.0f,0.0f},
		Vector3{-1.0f,1.0f,0.0f},
	};
	transform.set_translate({ 0,0,3 });
}

void Enemy::update() {
	move();
	arm_movement();
}

void Enemy::move() {
	for (int i = 0; i < ENEMY_NUM_PARTS; ++i) {
		partsInstance[i].get_transform().set_translate(basePartsOffset[i]);
	}
	const Quaternion& quaterinon = transform.get_quaternion();
	transform.set_rotate(
		quaterinon * Quaternion::AngleAxis(CVector3::BASIS_Y, PI / 6 * GameTimer::DeltaTime())
	);

	constexpr float MOVE_SPEED = 2.0f;
	Vector3 forward = CVector3::BASIS_Z * quaterinon;

	transform.plus_translate(
		forward * MOVE_SPEED * GameTimer::DeltaTime()
	);
}

void Enemy::arm_movement() {
	animationTimer += GameTimer::DeltaTime();
	constexpr float CYCLE = 1.0f;
	constexpr float ANGLE = PI2;
	constexpr Vector3 BASE_DISPLACEMENT = { 0,0.1f,0.0f };
	animationTimer = std::fmod(animationTimer, CYCLE);
	float parametric = animationTimer / CYCLE;
	Vector3 displacement = BASE_DISPLACEMENT * Quaternion::AngleAxis(CVector3::BASIS_X, parametric * ANGLE);
	partsInstance[EnemyParts::ArmL].get_transform().set_translate(basePartsOffset[EnemyParts::ArmL] + displacement);
	partsInstance[EnemyParts::ArmR].get_transform().set_translate(basePartsOffset[EnemyParts::ArmR] + displacement);
}
