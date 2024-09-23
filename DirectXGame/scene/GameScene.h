#pragma once

#include <BaseScene.h>

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"

#include "scene/game/Skydome/Skydome.h"
#include "scene/game/Player/Player.h"
#include "scene/game/Enemy/Enemy.h"
#include "scene/game/FollowCamera/FollowCamera.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public BaseScene {
public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void initialize() override;

	void begin() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void update() override;

	void matrix_update() override;

	void late_update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void draw() const override;

#ifdef _DEBUG
public:
	void debug_gui() override;
#endif // _DEBUG

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	std::unique_ptr<FollowCamera> camera;

	uint32_t textureHandle;
	//std::shared_ptr<Model> playerModel;
	std::shared_ptr<Model> playerModelHead;
	std::shared_ptr<Model> playerModelBody;
	std::shared_ptr<Model> playerModelArmL;
	std::shared_ptr<Model> playerModelArmR;
	std::shared_ptr<Model> skydomeModel;
	std::shared_ptr<Model> groundModel;
	std::shared_ptr<Model> enemyModelBody;
	std::shared_ptr<Model> enemyModelArm;

	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	std::unique_ptr<Skydome> skydome;
	std::unique_ptr<WorldInstance> ground;
};
