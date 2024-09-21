#pragma once

class BaseScene;

#include <memory>

class SceneManager {
private:
	SceneManager() = default;

public:
	~SceneManager() = default;

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

public:
	static SceneManager& GetInstance();

public:
	void initialize();
	void finalize();

	void begin();
	void update();
	void draw();

	void set_next_scene(std::unique_ptr<BaseScene> baseScene);

private:
	std::unique_ptr<BaseScene> nowScene;
	std::unique_ptr<BaseScene> nextScene;
};
