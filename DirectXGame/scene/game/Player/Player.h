#pragma once

#include "WorldInstance.h"

class Input;

class Player : public WorldInstance {
public: // コンストラクタ
	Player();
	~Player();

public: // publicメンバ関数
	void update();

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

public:

private: // メンバ変数
	Input* input;
};