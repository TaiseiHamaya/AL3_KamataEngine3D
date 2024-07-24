#include "GameScene.h"

#include <fstream>

#include "Sprite.h"
#include "TextureManager.h"
#include "AxisIndicator.h"

#include "Camera3D.h"

#include "Collider.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

GameScene::GameScene() {
}

GameScene::~GameScene() {
	delete debugCamera;
}

void GameScene::Initialize() {
	Camera3D::Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// いろいろ
	textureHandle = TextureManager::Load("uvChecker.png");
	reticleTextureHandle = TextureManager::Load("reticle.png");
	model = std::shared_ptr<Model>(Model::Create());
	skydomeModel = std::shared_ptr<Model>(Model::CreateFromOBJ("skydome", true));

	// 天球
	skydome = std::make_unique<Skydome>();
	skydome->initialize(skydomeModel);

	// カメラ
	railCamera = std::make_unique<RailCamera>();
	railCamera->initialize({ 0,0,-50 }, { 0,0,0 });

	// プレイヤー
	player = std::make_unique<Player>();
	player->initialize(model, textureHandle, reticleTextureHandle, { 0,0,50 });
	player->set_parent(railCamera->get_world_transform());
	player->set_viewprojection(&railCamera->get_vp());
	player->set_enemys(&enemys);

	// デバッグ
	isDebugCameraActive = false;
	debugCamera = new DebugCamera{ WinApp::kWindowWidth, WinApp::kWindowHeight };
	AxisIndicator::GetInstance()->SetVisible(true);

	EnemyBullet::SetPlayer(player.get());

	collisionManager = std::make_unique<CollisitonManager>();

	waitTime = 0;
	load_pop_data();
}

void GameScene::Update() {
	collisionManager->reset();
	update_pop_command();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_F1)) {
		isDebugCameraActive = !isDebugCameraActive;
	}
	if (isDebugCameraActive) {
		railCamera->update();
		if (!ImGui::GetIO().WantCaptureMouse) {
			debugCamera->Update();
		}
		railCamera->update_debug(debugCamera);
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
	for (auto enemyItr = enemys.begin(); enemyItr != enemys.end(); ++enemyItr) {
		enemyItr->update();
	}
	// enemyBullet
	for (auto bulletsItr = enemyBullets.begin(); bulletsItr != enemyBullets.end(); ++bulletsItr) {
		bulletsItr->update();
	}
	enemys.remove_if([](const Enemy& enemy) {
		if (enemy.is_dead()) {
			return true;
		}
		return false;
		});
	enemyBullets.remove_if([](const EnemyBullet& bullet) {
		if (bullet.is_dead()) {
			return true;
		}
		return false;
		});

	skydome->update();

	collision();
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
	for (auto enemyItr = enemys.begin(); enemyItr != enemys.end(); ++enemyItr) {
		enemyItr->draw(railCamera->get_vp());
	}
	skydome->draw(railCamera->get_vp());
	for (auto bulletsItr = enemyBullets.begin(); bulletsItr != enemyBullets.end(); ++bulletsItr) {
		bulletsItr->draw(railCamera->get_vp());
	}

	railCamera->draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// ---------------------------------------///
	/// ここに前景スプライトの描画処理を追加できる ///
	/// ---------------------------------------///

	player->draw_ui();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::collision() {
	std::list<PlayerBullet>& playerBullets = player->get_bullets();
	collisionManager->add_collider(player.get());
	for (auto playerBulletsItr = playerBullets.begin(); playerBulletsItr != playerBullets.end(); ++playerBulletsItr) {
		collisionManager->add_collider(&*playerBulletsItr);
	}
	for (auto enemyItr = enemys.begin(); enemyItr != enemys.end(); ++enemyItr) {
		collisionManager->add_collider(&*enemyItr);
	}
	for (auto enmeyBulletsItr = enemyBullets.begin(); enmeyBulletsItr != enemyBullets.end(); ++enmeyBulletsItr) {
		collisionManager->add_collider(&*enmeyBulletsItr);
	}
	collisionManager->collision();
}

void GameScene::add_enemy_bullets(Vector3&& position) {
	enemyBullets.emplace_back();
	enemyBullets.back().initialize(model, position, (player->get_position() - position).normalize());
}

void GameScene::load_pop_data() {
	std::ifstream file{};
	file.open("./Resources/Stage.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::update_pop_command() {
	--waitTime;
	if (waitTime > 0) {
		return;
	}
	std::string line;
	while (std::getline(enemyPopCommands, line)) {
		std::istringstream lineStream{ line };
		std::string word;

		std::getline(lineStream, word, ',');

		if (word == "//") {
			continue;
		}
		else if (word == "POP") {
			Vector3 popPosition;
			std::getline(lineStream, word, ',');
			popPosition.x = std::stof(word);
			std::getline(lineStream, word, ',');
			popPosition.y = std::stof(word);
			std::getline(lineStream, word, ',');
			popPosition.z = std::stof(word);

			pop_enemy(std::move(popPosition));
		}
		else if (word == "WAIT") {
			std::getline(lineStream, word, ',');
			waitTime = std::stoi(word);

			break;
		}
	}
}

void GameScene::pop_enemy(Vector3&& position) {
	enemys.emplace_back();
	enemys.rbegin()->initialize(model, std::move(position), textureHandle);
	enemys.rbegin()->set_player(player.get());
	enemys.rbegin()->set_game_scene(this);
}
