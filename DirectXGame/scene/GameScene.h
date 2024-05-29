#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"

#include "Skydome.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "RailCamera.h"

#include <memory>

#ifdef _DEBUG
#include "DebugCamera.h"
#endif // _DEBUG

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

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
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	int32_t textureHandle;
	std::shared_ptr<Model> model;
	std::shared_ptr<Model> skydomeModel;
	
	std::unique_ptr<Player> player;
	std::unique_ptr<Enemy> enemy;
	std::unique_ptr<Skydome> skydome;
	std::unique_ptr<RailCamera> railCamera;

	bool isDebugCameraActive;
	DebugCamera* debugCamera;
};
