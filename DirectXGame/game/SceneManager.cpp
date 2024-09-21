#include "SceneManager.h"

#include <BaseScene.h>

SceneManager& SceneManager::GetInstance() {
	static SceneManager instance{};
	return instance;
}

void SceneManager::initialize() {
	nowScene.reset();
	nextScene.reset();
}

void SceneManager::finalize() {
	nowScene.reset();
	nextScene.reset();
}

void SceneManager::begin() {
	if (nextScene) {
		nowScene = std::move(nextScene);
		nowScene->initialize();
	}
}

void SceneManager::update() {
	nowScene->begin();
	nowScene->update();
#ifdef _DEBUG
	nowScene->debug_gui();
#endif // _DEBUG
	nowScene->matrix_update();
	nowScene->late_update();
}

void SceneManager::draw() {
	nowScene->draw();
}

void SceneManager::set_next_scene(std::unique_ptr<BaseScene> baseScene) {
	nextScene = std::move(baseScene);
}
