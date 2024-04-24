#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include <vector>

/// <summary>
///	線形補間
/// </summary>
/// <typeparam name="type">型名</typeparam>
/// <param name="from">t=0のときの値</param>
/// <param name="to">t=1のときの値</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
template<typename type>
type Lerp(const type& from, const type& to, const float t);

/// <summary>
/// N次曲線の線形補間
/// </summary>
/// <param name="node">頂点情報</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector2 Bezier(const std::vector<Vector2>& node, const float t);

/// <summary>
/// N次曲線の線形補間
/// </summary>
/// <param name="node">頂点情報</param>
/// <param name="t">媒介変数</param>
/// <returns></returns>
Vector3 Bezier(const std::vector<Vector3>& node, const float t);

class Easing final {
public:
	class In {
	public:
		static float Sine(float t);
		static float Quad(float t);
		static float Cubic(float t);
		static float Quart(float t);
		static float Quint(float t);
		static float Expo(float t);
		static float Circ(float t);
		static float Back(float t);
		static float Elastic(float t);
		static float Bounce(float t);
	};
	class Out {
	public:
		static float Sine(float t);
		static float Quad(float t);
		static float Cubic(float t);
		static float Quart(float t);
		static float Quint(float t);
		static float Expo(float t);
		static float Circ(float t);
		static float Back(float t);
		static float Elastic(float t);
		static float Bounce(float t);
	};
	class InOut {
	public:
		static float Sine(float t);
		static float Quad(float t);
		static float Cubic(float t);
		static float Quart(float t);
		static float Quint(float t);
		static float Expo(float t);
		static float Circ(float t);
		static float Back(float t);
		static float Elastic(float t);
		static float Bounce(float t);
	};

private:

};

template<typename type>
inline type Lerp(const type& from, const type& to, const float t) {
	return type(from * (1 - t)) + type(to * t);
}
