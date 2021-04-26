
#ifndef CNGE_MATH
#define CNGE_MATH

#include "../types.h"

namespace CNGE {
	template<typename  T>
	constexpr T PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164;

	template<typename T>
	T mod(T a, T b) {
		return (b + (a % b)) % b;
	}

	template<>
	f32 mod<f32>(f32 a, f32 b);

	template<>
	f64 mod<f64>(f64 a, f64 b);

	template<typename T>
	i32 sign(T val) {
		return (val >= 0) ? 1 : -1;
	}

	template<typename T>
	auto interp(T start, T end, T along) -> T {
		return (end - start) * along + start;
	}

	template<typename T>
	auto interpSquared(T start, T end, T along) -> T {
		return (end - start) * (along * along) + start;
	}

	template<typename T>
	auto interpReverseSquared(T start, T end, T along) -> T {
		return (end - start) * ((T)1 - ((along - (T)1) * (along - (T)1))) + start;
	}

	template<typename T>
	auto invInterp(T start, T end, T value) -> T {
		return (value - start) / (end - start);
	}

	template<typename T>
	auto inclusiveRange(T low, T value, T high) -> bool {
		return (value >= low) && (value <= high);
	}

	template<typename T>
	auto exclusiveRange(T low, T value, T high) -> bool {
		return (value > low) && (value < high);
	}
}

#endif
