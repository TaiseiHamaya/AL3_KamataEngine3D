#include "GameScene.h"

#include "TextureManager.h"
#include "AxisIndicator.h"

#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textureHandle = TextureManager::Load("./Resources/uvChecker.png");
	model = std::shared_ptr<Model>(Model::Create());
	viewProjection.Initialize();

	player = std::make_unique<Player>();
	player->initialize(model, textureHandle);
	isDebugCameraActive = false;
	debugCamera = new DebugCamera{ WinApp::kWindowWidth, WinApp::kWindowHeight };
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F1)) {
		isDebugCameraActive = !isDebugCameraActive;
	}
	if (isDebugCameraActive) {
		debugCamera->Update();
		viewProjection.matView = debugCamera->GetViewProjection().matView;
		viewProjection.matProjection = debugCamera->GetViewProjection().matProjection;
		viewProjection.TransferMatrix();
	}
	else {
		viewProjection.UpdateMatrix();
	}
#endif // _DEBUG
	player->update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// -------------------------------------- ///
	/// ここに背景スプライトの描画処理を追加できる ///
	/// ---------------------------------------///

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// ---------------------------------------///
	/// ここに3Dオブジェクトの描画処理を追加できる ///
	/// ---------------------------------------///
	player->draw(viewProjection);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// ---------------------------------------///
	/// ここに前景スプライトの描画処理を追加できる ///
	/// ---------------------------------------///

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
