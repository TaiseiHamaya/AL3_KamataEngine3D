#pragma once

#include <WorldInstance.h>

template<uint32_t NUM_PARTS>
class MultiModelInstance : public WorldInstance {
	static_assert(NUM_PARTS >= 1);
public:
	MultiModelInstance() = default;
	~MultiModelInstance() = default;

public:
	MultiModelInstance(const MultiModelInstance&) = delete;
	MultiModelInstance& operator=(const MultiModelInstance&) = delete;
	MultiModelInstance(MultiModelInstance&&) = default;
	MultiModelInstance& operator=(MultiModelInstance&&) = default;


public: // publicメンバ関数
	void initialize() override;
	void matrix_update() override;
	void draw() const override;

public:
	void set_parts_models(std::array<std::shared_ptr<Model>, NUM_PARTS>  models_);

protected:
	std::array<WorldInstance, NUM_PARTS> partsInstance;
};

template<uint32_t NUM_PARTS>
inline void MultiModelInstance<NUM_PARTS>::initialize() {
	WorldInstance::initialize();
	for (WorldInstance& parts : partsInstance) {
		parts.initialize();
		parts.set_parent(*this);
	}
}

template<uint32_t NUM_PARTS>
inline void MultiModelInstance<NUM_PARTS>::matrix_update() {
	WorldInstance::matrix_update();
	for (WorldInstance& parts : partsInstance) {
		parts.matrix_update();
	}
}

template<uint32_t NUM_PARTS>
inline void MultiModelInstance<NUM_PARTS>::draw() const {
	WorldInstance::draw();
	for (const WorldInstance& parts : partsInstance) {
		parts.draw();
	}
}

template<uint32_t NUM_PARTS>
inline void MultiModelInstance<NUM_PARTS>::set_parts_models(std::array<std::shared_ptr<Model>, NUM_PARTS> models_) {
	for (int index = 0; const std::shared_ptr<Model>& playerModelParts : models_) {
		partsInstance[index].set_model(playerModelParts);
		++index;
	}
}
