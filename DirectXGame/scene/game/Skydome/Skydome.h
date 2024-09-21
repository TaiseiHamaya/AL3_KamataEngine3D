#pragma once

#include "WorldInstance.h"

class Skydome : public WorldInstance {
public:
	Skydome() = default;
	~Skydome() = default;

public:
	void initialize() override;

private:
};

