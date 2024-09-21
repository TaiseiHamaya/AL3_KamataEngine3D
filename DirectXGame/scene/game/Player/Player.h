#pragma once

#include "WorldInstance.h"

#include "Input.h"

#include <array>

class Camera3D;

class Player : public WorldInstance {
private:
	enum PlayerParts {
		Body,
		Head,
		ArmL,
		ArmR,
	};

	static constexpr size_t NUM_PARTS = 4;

public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void initialize() override;
	void update() override;
	void matrix_update() override;
	void draw() const override;

	void input(const XINPUT_STATE& joyState);

private:
	void move();
	void rotation();
	void floating();
	void arm_swing();

public:
	void set_camera(const Camera3D* camera_);
	void set_models(std::array<std::shared_ptr<Model>, NUM_PARTS>  models_);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

public:

private: // メンバ変数
	Vector3 velocity;

	Vector2 inputStickL;
	const Camera3D* camera;

	float floatingParameter;

	float swingTimer;

	std::array<WorldInstance, NUM_PARTS> playerParts;
	std::array<Vector3, NUM_PARTS> basePartsOffset;
};