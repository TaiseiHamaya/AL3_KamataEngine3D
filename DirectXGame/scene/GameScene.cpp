#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

#include <imgui.h>
#include <PrimitiveDrawer.h>
#include <AxisIndicator.h>

GameScene::GameScene() {
}

GameScene::~GameScene() {
	delete sprite;
	delete model;
	delete debugCamera;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle = TextureManager::Load("uvChecker.png");
	sprite = Sprite::Create(textureHandle, Vector2{0, 0});
	model = Model::Create();
	worldTransform.Initialize();
	viewProjection.Initialize();
	soundDataHandle = audio_->LoadWave("fanfare.wav");
	voiceHandle = audio_->PlayWave(soundDataHandle, true);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
	debugCamera = new DebugCamera(1280, 720);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera->GetViewProjection());
}

void GameScene::Update() {
	Vector2 position = sprite->GetPosition();
	position += Vector2{2.0f, 1.0f};
	sprite->SetPosition(position);
	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->StopWave(voiceHandle);
	}

	ImGui::Begin("TextBegin");
	ImGui::Text("Text");
	ImGui::DragFloat2("DragFloat2", &position.x);
	ImGui::End();

	debugCamera->Update();
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
	model->Draw(worldTransform, debugCamera->GetViewProjection(), textureHandle);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite->Draw();
	PrimitiveDrawer::GetInstance()->DrawLine3d(Vec3::kZero, {0, 10, 0}, {1.0f, 1.0f, 1.0f, 1.0f});

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
