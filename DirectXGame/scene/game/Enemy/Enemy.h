#pragma once

#include <MultiModelInstance.h>
#include <array>

static constexpr size_t ENEMY_NUM_PARTS = 3;

class Enemy : public MultiModelInstance<ENEMY_NUM_PARTS> {
private:
	enum EnemyParts {
		Body,
		ArmL,
		ArmR
	};

public:
	Enemy() = default;
	~Enemy() = default;

public:
	void initialize() override;
	void update() override;

private:
	void move();
	void arm_movement();

private:
	std::array<Vector3, ENEMY_NUM_PARTS> basePartsOffset;

	float animationTimer;
};
