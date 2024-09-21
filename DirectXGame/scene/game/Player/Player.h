#pragma once

#include "WorldInstance.h"

#include "Input.h"

class Camera3D;

class Player : public WorldInstance {
public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void update();

	void input(const XINPUT_STATE& joyState);

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
};