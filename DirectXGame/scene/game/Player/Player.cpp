#include "Player.h"

#include <Input.h>
#include "Transform3D.h"

#include <cassert>

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG

Player::Player() = default;

Player::~Player() = default;

void Player::update() {
}

#ifdef _DEBUG
void Player::debug_gui() {
	ImGui::Begin("Player", nullptr);
	transform.debug_gui();
	ImGui::End();
}
#endif // _DEBUG
