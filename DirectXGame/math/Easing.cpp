#include "Easing.h"
#include "Definition.h"

//----------------In----------------
float Easing::In::Sine(float t) {
	return 1.0f - cosf(PI * t / 2);
}

float Easing::In::Quad(float t) {
	return t * t;
}

float Easing::In::Cubic(float t) {
	return t * t * t;
}

float Easing::In::Quart(float t) {
	return t * t * t * t;
}

float Easing::In::Quint(float t) {
	return t * t * t * t * t;
}

float Easing::In::Expo(float t) {
	return t == 0 ? 0 : powf(2, t * 10 - 10);
}

float Easing::In::Circ(float t) {
	return 1 - sqrtf(1 - powf(t, 2));
}

float Easing::In::Back(float t) {
	return 2.70158f * t * t * t - 1.70158f * t * t;
}

float Easing::In::Elastic(float t) {
	return t == 0 ? 0
		: t == 1 ? 1
		: -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75f) * (2 * PI) / 3);
}

float Easing::In::Bounce(float t) {
	return 1 - Easing::Out::Bounce(1 - t);
}

//----------------Out----------------
float Easing::Out::Sine(float t) {
	return sinf(PI * t / 2);
}

float Easing::Out::Quad(float t) {
	return 1 - Easing::In::Quad(1 - t);
}

float Easing::Out::Cubic(float t) {
	return 1 - Easing::In::Cubic(1 - t);
}

float Easing::Out::Quart(float t) {
	return 1 - Easing::In::Quart(1 - t);
}

float Easing::Out::Quint(float t) {
	return 1 - Easing::In::Quint(1 - t);
}

float Easing::Out::Expo(float t) {
	return t == 1 ? 1 : 1 - powf(2, t * -10);
}

float Easing::Out::Circ(float t) {
	return sqrtf(1 - powf(t - 1, 2));
}

float Easing::Out::Back(float t) {
	return 1 + 2.70158f * (t - 1) * (t - 1) * (t - 1) + 1.70158f * (t - 1) * (t - 1);
}

float Easing::Out::Elastic(float t) {
	return t == 0 ? 0
		: t == 1 ? 1
		: powf(2, -10 * t) * sinf((t * 10 - 0.75f) * (2 * PI) / 3) + 1;
	;
}

float Easing::Out::Bounce(float t) {
	if (t < (1 / 2.75f)) {
		return 7.5625f * t * t;
	}
	else if (t < 2 / 2.75f) {
		return 7.5625f * (t -= 1.5f / 2.75f) * t + 0.75f;
	}
	else if (t < 2.5 / 2.75f) {
		return 7.5625f * (t -= 2.25f / 2.75f) * t + 0.9375f;
	}
	else {
		return 7.5625f * (t -= 2.625f / 2.75f) * t + 0.984375f;
	};
}

//----------------InOut----------------

float Easing::InOut::Sine(float t) {
	return -(cosf(PI * t) - 1) / 2;
}

float Easing::InOut::Quad(float t) {
	return t < 0.5 ? 2 * Easing::In::Quad(t) : 1 - Easing::In::Quad(-2 * t + 2) / 2;
}

float Easing::InOut::Cubic(float t) {
	return t < 0.5 ? 4 * Easing::In::Cubic(t) : 1 - Easing::In::Cubic(-2 * t + 2) / 2;
}

float Easing::InOut::Quart(float t) {
	return t < 0.5 ? 8 * Easing::In::Quart(t) : 1 - Easing::In::Quart(-2 * t + 2) / 2;
}

float Easing::InOut::Quint(float t) {
	return t < 0.5 ? 16 * Easing::In::Quint(t) : 1 - Easing::In::Quint(-2 * t + 2) / 2;
}

float Easing::InOut::Expo(float t) {
	return t < 0.5 ? Easing::In::Expo(2 * t) / 2 : (Easing::Out::Expo(2 * t - 1) + 1) / 2;
}

float Easing::InOut::Circ(float t) {
	return t < 0.5 ? Easing::In::Circ(t * 2) / 2 : (Easing::Out::Circ(-2 * t + 3) + 1) / 2;
}

float Easing::InOut::Back(float t) {
	return t < 0.5 ? (powf(2 * t, 2) * ((2.5949095f + 1) * 2 * t - 2.5949095f)) / 2
		: (powf(2 * t - 2, 2) * ((2.5949095f + 1) * (t * 2 - 2) + 2.5949095f) + 2) / 2;
}

float Easing::InOut::Elastic(float t) {
	return t == 0 ? 0
		: t == 1 ? 1
		: t < 0.5
		? -(powf(2, 20 * t - 10) * sinf((20 * t - 11.125f) * (2 * PI) / 4.5f)) / 2
		: (powf(2, -20 * t + 10) * sinf((20 * t - 11.125f) * (2 * PI) / 4.5f)) / 2 +
		1;
}

float Easing::InOut::Bounce(float t) {
	return t < 0.5 ? (1 - Easing::Out::Bounce(1 - 2 * t)) / 2
		: (1 + Easing::Out::Bounce(2 * t - 1)) / 2;
}

Vector2 Bezier(const std::vector<Vector2>& node, const float t) {
	std::vector<Vector2> nextNode(node.size() - 1);
	if (node.size() == 2) {
		return Lerp(node[0], node[1], t);
	}
	else {
		for (int i = 0; i < nextNode.size(); i++) {
			nextNode[i] = Lerp(node[i], node[i + 1], t);
		}
		return Bezier(nextNode, t);
	}
}

Vector3 Bezier(const std::vector<Vector3>& node, const float t) {
	std::vector<Vector3> nextNode(node.size() - 1);
	if (node.size() == 2) {
		return Lerp(node[0], node[1], t);
	}
	else {
		for (int i = 0; i < nextNode.size(); i++) {
			nextNode[i] = Lerp(node[i], node[i + 1], t);
		}
		return Bezier(nextNode, t);
	}
}
