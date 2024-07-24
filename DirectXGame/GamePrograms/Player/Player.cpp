#include "Player.h"

#include <cassert>
#include <Input.h>
#include <ViewProjection.h>
#include <math/Transform3D.h>

#include "Enemy/Enemy.h"
#include "Reticle.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Player::Player() = default;

Player::~Player() = default;

void Player::initialize(const std::shared_ptr<Model>& model_, uint32_t textureHandle_, uint32_t reticleSpriteHandle, Vector3&& position) {
	assert(model_);
	model = model_;
	textureHandle = textureHandle_;
	worldTransform.Initialize();
	worldTransform.translation_ = position;

	bullets.clear();

	reticle = std::make_unique<Reticle>();
	reticle->initialize(reticleSpriteHandle);

	input = Input::GetInstance();

	set_radius(2.5f);
	collisionAttribute = 0b0001;
	collisionMask = 0b1000;
}

void Player::update() {
	// imgui debug
#ifdef _DEBUG
	ImGui::SetNextWindowPos({ 50,250 }, ImGuiCond_Once);
	ImGui::SetNextWindowSize({ 210,100 }, ImGuiCond_Once);
	ImGui::Begin("Player", nullptr, ImGuiWindowFlags_NoSavedSettings);
	ImGui::DragFloat3("Scale", &worldTransform.scale_.x, 0.1f);
	ImGui::DragFloat3("Rotate", &worldTransform.rotation_.x, 0.02f);
	ImGui::DragFloat3("Position", &worldTransform.translation_.x, 0.5f);
	ImGui::End();
#endif // _DEBUG
	// input
	Vector3 moveStickL;
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState)) {
		moveStickL.x += float(joyState.Gamepad.sThumbLX) / (std::numeric_limits<short>::max)();
		moveStickL.y += float(joyState.Gamepad.sThumbLY) / (std::numeric_limits<short>::max)();
	}
	if (input->PushKey(DIK_LEFT)) {
		moveStickL.x -= 1;
	}
	if (input->PushKey(DIK_RIGHT)) {
		moveStickL.x += 1;
	}
	if (input->PushKey(DIK_UP)) {
		moveStickL.y += 1;
	}
	if (input->PushKey(DIK_DOWN)) {
		moveStickL.y -= 1;
	}

	// moveStickL
	if (moveStickL != CVector3::ZERO) {
		worldTransform.translation_ += moveStickL.normalize() * kCharacterSpeed;
	}
	worldTransform.translation_ = Vector3::Clamp(worldTransform.translation_, Vector3{ -33.0f, -18.0f, -10000.0f }, Vector3{ 33.0f, 18.0f, 10000.0f });

	// rotate
	if (input->PushKey(DIK_A)) {
		worldTransform.rotation_.y -= kCharacterRotateSpeed;
	}
	if (input->PushKey(DIK_D)) {
		worldTransform.rotation_.y += kCharacterRotateSpeed;
	}

	// update
	worldTransform.UpdateMatrix();

	reticle->update();

	// attak
	--attackTimer;
	if (attackTimer <= 0) {
		attack();
	}

	// bullet update
	for (auto bullet_itr = bullets.begin(); bullet_itr != bullets.end(); ++bullet_itr) {
		bullet_itr->update();
	}

	bullets.remove_if([](const PlayerBullet& bullet) {
		if (bullet.is_dead()) {
			return true;
		}
		return false;
		});
}

void Player::draw(const ViewProjection& viewProjection) const {
	model.lock()->Draw(worldTransform, viewProjection, textureHandle);
	//model.lock()->Draw(transform3DReticle, viewProjection, textureHandle);
	for (auto bullet_itr = bullets.begin(); bullet_itr != bullets.end(); ++bullet_itr) {
		bullet_itr->draw(viewProjection);
	}
}

void Player::draw_ui() {
	reticle->draw_ui();
}

void Player::on_collision() {
	// do nothing
}

void Player::attack() {
	XINPUT_STATE joyState;
	if (input->GetJoystickState(0, joyState) && (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		if (reticle->is_lockon()) {
			auto&& lockonList = reticle->get_lockon_list();
			for (auto&& lockonItr = lockonList.begin(); lockonItr != lockonList.end(); ++lockonItr) {
				add_bullet(lockonItr->first->get_position());
			}
			reticle->clear_lockon();
			attackTimer = 30;
		}
		else {
			add_bullet(reticle->get_position());
			attackTimer = 5;
		}
	}
}

void Player::add_bullet(Vector3&& targetPosition) {
	static constexpr float kBulletSpeed = 3.0f;
	Vector3 velocity = (targetPosition - get_position()).normalize() * kBulletSpeed;
	bullets.emplace_back();
	bullets.back().initialize(model, get_position(), velocity);
}

Vector3 Player::get_position() const {
	return Transform3D::ExtractPosition(worldTransform.matWorld_);
}

std::list<PlayerBullet>& Player::get_bullets() {
	return bullets;
}

const std::list<PlayerBullet>& Player::get_bullets() const {
	return bullets;
}

void Player::set_enemys(const std::list<Enemy>* const enemy) {
	reticle->set_enemy(enemy);
}

void Player::set_parent(const WorldTransform* parent) {
	worldTransform.parent_ = parent;
}

void Player::set_viewprojection(const ViewProjection* const viewProjection) {
	reticle->set_viewprojection(viewProjection);
}
