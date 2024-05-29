#include "GameScene.h"

#include "Sprite.h"
#include "TextureManager.h"
#include "AxisIndicator.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

bool IsCollision(const Vector3& pos1, const Vector3& pos2) {
	return (pos1 - pos2).length() < 3.0f;
}

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete debugCamera;
}

void GameScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// いろいろ
	textureHandle = TextureManager::Load("./Resources/uvChecker.png");
	model = std::shared_ptr<Model>(Model::Create());
	skydomeModel = std::shared_ptr<Model>(Model::CreateFromOBJ("skydome", true));

	// プレイヤー
	player = std::make_unique<Player>();
	player->initialize(model, textureHandle, {0,0,50});

	// エネミー
	enemy = std::make_unique<Enemy>(player.get());
	enemy->initialize(model, { 0,5,120 }, textureHandle);

	// 天球
	skydome = std::make_unique<Skydome>();
	skydome->initialize(skydomeModel);

	// カメラ
	railCamera = std::make_unique<RailCamera>();
	railCamera->initialize({ 0,0,0 }, { 0,0,0 });

	player->set_parent(railCamera->get_world_transform());

	// デバッグ
	isDebugCameraActive = false;
	debugCamera = new DebugCamera{ WinApp::kWindowWidth, WinApp::kWindowHeight };
	AxisIndicator::GetInstance()->SetVisible(true);
}

void GameScene::Update() {
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F1)) {
		isDebugCameraActive = !isDebugCameraActive;
	}
	if (isDebugCameraActive) {
		if (!ImGui::GetIO().WantCaptureMouse) {
			debugCamera->Update();
			railCamera->update_debug(debugCamera);
		}
	}
	else {
		railCamera->update();
	}
#else
	railCamera->update();
#endif
	// player
	player->update();

	// enemy
	enemy->update();

	skydome->update();

	std::list<PlayerBullet>& playerBullets = player->get_bullets();
	std::list<EnemyBullet>& enemyBullets = enemy->get_bullets();
	for (auto playerBulletsItr = playerBullets.begin(); playerBulletsItr != playerBullets.end(); ++playerBulletsItr) {
		if (IsCollision(enemy->get_position(), playerBulletsItr->get_position())) {
			enemy->on_collision();
			playerBulletsItr->on_collision();
		}
	}
	for (auto enmeyBulletsItr = enemyBullets.begin(); enmeyBulletsItr != enemyBullets.end(); ++enmeyBulletsItr) {
		if (IsCollision(player->get_position(), enmeyBulletsItr->get_position())) {
			player->on_collision();
			enmeyBulletsItr->on_collision();
		}
	}
	for (auto enmeyBulletsItr = enemyBullets.begin(); enmeyBulletsItr != enemyBullets.end(); ++enmeyBulletsItr) {
		for (auto playerBulletsItr = playerBullets.begin(); playerBulletsItr != playerBullets.end(); ++playerBulletsItr) {
			if (IsCollision(playerBulletsItr->get_position(), enmeyBulletsItr->get_position())) {
				enmeyBulletsItr->on_collision();
				playerBulletsItr->on_collision();
			}
		}
	}
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
	player->draw(railCamera->get_vp());
	enemy->draw(railCamera->get_vp());
	skydome->draw(railCamera->get_vp());

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
