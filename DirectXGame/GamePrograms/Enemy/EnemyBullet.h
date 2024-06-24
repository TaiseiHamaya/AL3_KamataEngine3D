#pragma once

#include <Model.h>
#include <ViewProjection.h>
#include <WorldTransform.h>
#include <memory>

#include "Collider.h"

class Player;

class EnemyBullet : public Collider {
public:
	void initialize(std::weak_ptr<Model> p_model, const Vector3& position, const Vector3& velocity_);
	void update();
	void draw(const ViewProjection& viewProjection) const;

public:
	bool is_dead() const { return isDead; }
	void on_collision() override;
	Vector3 get_position() const override;

private:
	void set_direction();

private:
	static constexpr std::uint32_t LifeTime = 60 * 5;

private:
	WorldTransform transform;
	std::weak_ptr<Model> model;
	std::uint32_t textureHandle;
	Vector3 velocity;
	std::uint32_t deathTimer;
	static constexpr float speed = 0.5f;
	bool isDead;

public:
	static void SetPlayer(Player* plyaer_);

private:
	static Player* player;
};
