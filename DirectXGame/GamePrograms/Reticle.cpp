#include "Reticle.h"

#include <Input.h>
#include <WinApp.h>
#include <ViewProjection.h>

#include "Transform3D.h"
#include "Camera3D.h"

#include "Enemy/Enemy.h"

void Reticle::initialize(uint32_t textureHandle) {
	sprite.reset(Sprite::Create(textureHandle, { 0,0 }, { 0.0f, 0.0f, 0.0f, 1.0f }));
	sprite->SetAnchorPoint({ 0.5f, 0.5f });

	reticlePos = { static_cast<float>(WinApp::kWindowWidth / 2),static_cast<float>(WinApp::kWindowHeight / 2) };

	transform3DReticle.Initialize();
}

void Reticle::update() {
	if (lockon) {
		bool isAliveTarget = false;
		for (auto&& enemyItr = enemys->begin(); enemyItr != enemys->end(); ++enemyItr) {
			if (&*enemyItr == lockon) {
				isAliveTarget = true;
				break;
			}
		}
		if (!isAliveTarget) {
			sprite->SetColor({0.0f, 0.0f, 0.0f, 1.0f});
			lockon = nullptr;
		}
		else {
			Vector3 newPosition = Transform3D::Homogeneous(lockon->get_position(), refViewProjection->matView * refViewProjection->matProjection * Camera3D::GetViewPortMatrix());
			reticlePos = { newPosition.x, newPosition.y };
		}
	}
	else {
		Input* input = Input::GetInstance();
		XINPUT_STATE joyState;
		// 2Dレティクル
		Vector2 moveStickR{};
		if (input->GetJoystickState(0, joyState)) {
			moveStickR.x += float(joyState.Gamepad.sThumbRX) / (std::numeric_limits<short>::max)();
			moveStickR.y -= float(joyState.Gamepad.sThumbRY) / (std::numeric_limits<short>::max)();
		}
		static constexpr float kReticleMoveSpeed = 8;
		reticlePos += moveStickR * kReticleMoveSpeed;
		reticlePos = Vector2::Clamp(reticlePos, { 64, 64 }, { 1216, 656 });

		Matrix4x4 vpvMat = refViewProjection->matView * refViewProjection->matProjection * Camera3D::GetViewPortMatrix();
		for (auto&& enemyItr = enemys->begin(); enemyItr != enemys->end(); ++enemyItr) {
			Vector3 enemyScreenPos = Transform3D::Homogeneous(enemyItr->get_position(), vpvMat);
			if (enemyScreenPos.z <= 1 && enemyScreenPos.z >= 0 && (reticlePos - Vector2{ enemyScreenPos.x, enemyScreenPos.y }).length() < 60) {
				lockon = std::to_address(enemyItr);
				sprite->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
				break;
			}
		}
	}

	sprite->SetPosition(reticlePos);

	// 球の狙い位置
	if (lockon) {
		transform3DReticle.translation_ = lockon->get_position();
	}
	else {
		Matrix4x4 matrix = (refViewProjection->matView * refViewProjection->matProjection * Camera3D::GetViewPortMatrix()).inverse();
		Vector3 posNear = Transform3D::Homogeneous({ reticlePos.x, reticlePos.y, 0 }, matrix);
		Vector3 posFar = Transform3D::Homogeneous({ reticlePos.x, reticlePos.y, 1 }, matrix);
		static constexpr float kReticleDistance = 100.0f;
		Vector3 mouseDirection = (posFar - posNear).normalize() * kReticleDistance;
		transform3DReticle.translation_ = posNear + mouseDirection;
	}
	transform3DReticle.UpdateMatrix();
}

void Reticle::draw_ui() const {
	sprite->Draw();
}

Vector3 Reticle::get_position() const {
	return Transform3D::ExtractPosition(transform3DReticle.matWorld_);
}

void Reticle::set_viewprojection(const ViewProjection* const viewProjection) {
	refViewProjection = viewProjection;
}

void Reticle::set_enemy(const std::list<Enemy>* const enemys_) {
	enemys = enemys_;
}
