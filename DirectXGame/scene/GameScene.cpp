#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include "Camera3D.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	camera = std::make_unique<Camera3D>();
	camera->initialize();

	// いろいろ
	//textureHandle = TextureManager::Load("uvChecker.png");
	playerModel = std::shared_ptr<Model>(Model::CreateFromOBJ("player", true));
	skydomeModel = std::shared_ptr<Model>(Model::CreateFromOBJ("skydome", true));
	groundModel = std::shared_ptr<Model>(Model::CreateFromOBJ("ground", true));

	// 天球
	skydome = std::make_unique<Skydome>();
	skydome->initialize();
	skydome->set_model(skydomeModel);

	// 地面
	ground = std::make_unique<WorldInstance>();
	ground->initialize();
	ground->set_model(groundModel);

	// プレイヤー
	player = std::make_unique<Player>();
	player->initialize();
	player->set_model(playerModel);

	WorldInstance::SetStaticViewProjection(camera->get_view_projection());
}

void GameScene::begin() {
}

void GameScene::update() {
	camera->update();
	player->update();
	skydome->update();
	ground->update();
}

void GameScene::matrix_update() {
	camera->matrix_update();
	player->matrix_update();
	skydome->matrix_update();
	ground->matrix_update();
}

void GameScene::late_update() {
	camera->late_update();
	player->late_update();
	skydome->late_update();
	ground->late_update();
}

void GameScene::draw() const {
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

	player->draw();
	skydome->draw();
	ground->draw();

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

#ifdef _DEBUG

void GameScene::debug_gui() {
	player->debug_gui();
	camera->debug_gui();
	camera->debug_camera();
}
#endif // _DEBUG
