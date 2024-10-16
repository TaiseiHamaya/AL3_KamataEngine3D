#pragma once

#include "Input.h"
#include "MultiModelInstance.h"

#include <array>
#include <optional>
#include <variant>

class Camera3D;

static constexpr size_t PLAYER_NUM_PARTS = 5;

class Player : public MultiModelInstance<PLAYER_NUM_PARTS> {
private:
	enum PlayerParts {
		Body,
		Head,
		ArmL,
		ArmR,
		Hammer,
	};

	enum class PlayerBehavior {
		Root,
		Attack,
		Dash
	};

	struct WorkRoot {
		float swingTimer;
	};

	struct WorkAttack {
		float timer;
	};

	struct WorkDash {
		float timer;
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

	void behavior_update();
	void behavior_root_initialize();
	void behavior_root_update();
	void behavior_attack_initialize();
	void behavior_attack_update();
	void behavior_dash_initialize();
	void behavior_dash_update();

	void add_global_value();
	void get_global_value();

public:
	void set_camera(const Camera3D* camera_);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private: // メンバ変数
	Vector3 velocity;

	Vector2 inputStickL;
	bool isPressA;
	bool isPressRB;
	const Camera3D* camera;

	float floatingParameter;

	std::array<Vector3, PLAYER_NUM_PARTS> basePartsOffset;

	PlayerBehavior behavior;
	std::optional<PlayerBehavior> behaviorRequest;
	std::variant<WorkRoot, WorkAttack, WorkDash > behaviorValue;
};
