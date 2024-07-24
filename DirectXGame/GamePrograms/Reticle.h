#pragma once

#include <memory>
#include <list>
#include <unordered_map>

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
	bool is_lockon() const;
	const std::unordered_map<const Enemy*, std::unique_ptr<Sprite>>& get_lockon_list() const;
	void clear_lockon();

private:
	WorldTransform transform3DReticle;
	std::unique_ptr<Sprite> sprite;
	Vector2 reticlePos;

	const ViewProjection* refViewProjection;
	const std::list<Enemy>* enemys;
	std::uint32_t textureHandle;

	std::unordered_map<const Enemy*, std::unique_ptr<Sprite>> lockon;
};
