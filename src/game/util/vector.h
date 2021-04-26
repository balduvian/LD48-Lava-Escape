//
// Created by Emmet on 8/22/2020.
//

#ifndef CNGE_8_2_VECTOR_H
#define CNGE_8_2_VECTOR_H

#include "cnge/types.h"

namespace Game {
	class Vector {
	private:
		f32 x, y;

	public:
		Vector(f32, f32);

		Vector(const Vector&);
		auto operator=(const Vector&) -> Vector&;

		auto operator+(f32) const -> Vector;
		auto operator-(f32) const -> Vector;
		auto operator*(f32) const -> Vector;
		auto operator/(f32) const -> Vector;

		auto operator+(const Vector&) const -> Vector;
		auto operator-(const Vector&) const -> Vector;

		auto dot(Vector) -> f32;
		auto dot(f32, f32) -> f32;
		auto lengthSquared() -> f32;
		auto length() -> f32;
		auto angle() -> f32;
		auto getX() -> f32;
		auto getY() -> f32;

		/* mutators */

		auto set(f32, f32) -> Vector&;
		auto addX(f32) -> Vector&;
		auto addY(f32) -> Vector&;
		auto setX(f32) -> Vector&;
		auto setY(f32) -> Vector&;
		auto normalizeAngle(f32) -> Vector&;
		auto setAngle(f32) -> Vector&;
		auto rotate(f32) -> Vector&;
		auto negate() -> Vector&;
		auto project(Vector) -> Vector&;
		auto project(f32, f32) -> Vector&;
		auto normalize() -> Vector&;
		auto setLength(f32) -> Vector&;
	};
}

#endif //CNGE_8_2_VECTOR_H
