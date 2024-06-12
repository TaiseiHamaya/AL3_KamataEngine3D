#pragma once

#include <functional>

template<class Type>
class TimedCall {
public:
	TimedCall(std::function<Type> function_, std::uint32_t time_);

	void update();

	bool is_finished();

private:
	std::function<Type> function;
	std::uint32_t time;
	bool isFinished = false;
};

template<class Type>
inline TimedCall<Type>::TimedCall(std::function<Type> function_, std::uint32_t time_) {
	function = function_;
	time = time_;
}

template<class Type>
inline void TimedCall<Type>::update() {
	--time;
	if (time <= 0) {
		isFinished = true;
		function();
	}
}

template<class Type>
inline bool TimedCall<Type>::is_finished() {
	return isFinished;
}
