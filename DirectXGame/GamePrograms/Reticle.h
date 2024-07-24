#pragma once

#include <memory>
#include <list>
#include <unordered_set>

#include <WorldTransform.h>
#include <Sprite.h>

class ViewProjection;
class Enemy;

class Reticle {
public:
	Reticle() = default;
	~Reticle() = default;

public:
	void initialize(uint32_t textureHandle);
	void update();
	void draw_ui() const;

public:
	Vector3 get_position() const;
	void set_viewprojection(const ViewProjection* const viewProjection);
	void set_enemy(const std::list<Enemy>* const enemys);

private:
	WorldTransform transform3DReticle;
	std::unique_ptr<Sprite> sprite;
	Vector2 reticlePos;

	const ViewProjection* refViewProjection;
	const std::list<Enemy>* enemys;
	const Enemy* lockon;
};
