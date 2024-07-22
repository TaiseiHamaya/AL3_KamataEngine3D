#pragma once

#include <cassert>
#include <numbers>
#include <vector>
#include <cmath>
#include <algorithm>

#include "Matrix.h"
#include "Vector2.h"

constexpr float PI = float(std::numbers::pi);
constexpr float PI2 = float(std::numbers::pi * 2.0);

const Vector2 operator*(const Vector2& vec, const Matrix<2, 2>& mat);

/// <summary>
/// メモリのdeleteとnullptr化
/// <param name="mem">開放するメモリの変数</param>
/// </summary>
template<class type>
void MemClear(type*& mem);

template<class Type>
Type CatmullRom(const Type& p0, const Type& p1, const Type& p2, const Type& p3, float t);

template<class Type>
Type CatmullRom(std::vector<Type>& points, float t);

///-------------------///
///   ここから定義     ///
///-------------------///

template<class type>
inline void MemClear(type*& mem) {
	delete mem;
	mem = nullptr;
}

template<class Type>
inline Type CatmullRom(const Type& p0, const Type& p1, const Type& p2, const Type& p3, float t) {
	return (
		(-p0 + p1 * 3 - p2 * 3 + p3) * t * t * t +
		(p0 * 2 - p1 * 5 + p2 * 4 - p3) * t * t +
		(-p0 + p2) * t +
		p1 * 2) / 2;
}

template<class Type>
inline Type CatmullRom(std::vector<Type>& points, float t) {
	assert(points.size() >= 4);
	uint32_t division = static_cast<std::uint32_t > (points.size() - 1);

	float interveningWidth = std::fmod(t * division, 1.0f) + static_cast<int32_t>(t);

	uint32_t baseIndex = t < 1 ? static_cast<uint32_t>(t * division) : division - 1;

	uint32_t index0 = baseIndex != 0 ? baseIndex - 1 : 0;
	uint32_t index1 = baseIndex;
	uint32_t index2 = baseIndex + 1;
	uint32_t index3 = index2 < division ? baseIndex + 2 : index2;

	return CatmullRom(points[index0], points[index1], points[index2], points[index3], interveningWidth);
}

class Animation {
public:
	Animation();
	Animation(int distance_, int total_seq_, bool is_loop_);

public:
	int now(int time) const {
		return is_loop ? time / distance % total_seq : (std::min)(time / distance, total_seq);
	}

	bool is_end(int time) const {
		return time / distance >= total_seq;
	}

public:
	int distance;
	int total_seq;
	bool is_loop;
};