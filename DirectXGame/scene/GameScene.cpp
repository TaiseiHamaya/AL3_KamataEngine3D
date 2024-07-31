#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "Camera3D.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {
	Camera3D::Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// いろいろ
	textureHandle = TextureManager::Load("uvChecker.png");
	model = std::shared_ptr<Model>(Model::Create());
	//skydomeModel = std::shared_ptr<Model>(Model::CreateFromOBJ("skydome", true));

	// 天球
	//skydome = std::make_unique<Skydome>();
	//skydome->initialize(skydomeModel);

	// プレイヤー
	player = std::make_unique<Player>();
	player->initialize(model, textureHandle, { 0,0,0 });

	viewProjection = std::make_unique<ViewProjection>();
	viewProjection->Initialize();
}

void GameScene::Update() {
	Camera3D::DebugGUI();
	Camera3D::CameraUpdate();
	viewProjection->matView = Camera3D::GetViewMatrix();
	viewProjection->matProjection = Camera3D::GetProjectionMatrix();
	viewProjection->TransferMatrix();
	player->update();
}

void GameScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player->draw(*viewProjection);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
