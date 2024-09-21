#pragma once

#include <chrono>

class GameTimer {
private:
	GameTimer() = default;

public:
	~GameTimer() = default;

	GameTimer(const GameTimer&) = delete;
	GameTimer& operator=(const GameTimer&) = delete;

public:
	static GameTimer& GetInstance();
	static void Initialize();
	static void Update();
	static float DeltaTime();

private:
	float deltaTime;
	std::chrono::system_clock::time_point preFrameTime;
};

