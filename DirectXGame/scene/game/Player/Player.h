#pragma once

#include "MultiModelInstance.h"
#include "Input.h"

#include <array>

class Camera3D;

static constexpr size_t PLAYER_NUM_PARTS = 4;

class Player : public MultiModelInstance<PLAYER_NUM_PARTS> {
private:
	enum PlayerParts {
		Body,
		Head,
		ArmL,
		ArmR,
	};

public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void initialize() override;
	void update() override;

	void input(const XINPUT_STATE& joyState);

private:
	void move();
	void rotation();
	void floating();
	void arm_swing();

public:
	void set_camera(const Camera3D* camera_);

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

	std::array<Vector3, PLAYER_NUM_PARTS> basePartsOffset;
};
