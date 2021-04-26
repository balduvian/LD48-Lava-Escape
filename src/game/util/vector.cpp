//
// Created by Emmet on 8/22/2020.
//

#include <cmath>
#include "vector.h"

namespace Game {
	Vector::Vector(f32 x, f32 y) : x(x), y(y) {}

	Vector::Vector(const Vector &other) : x(other.x), y(other.y) {}

	auto Vector::operator=(const Vector &other) -> Vector& {
		this->x = other.x;
		this->y = other.y;
	}

	auto Vector::operator+(f32 v) const -> Vector {
		return Vector(x + v, y + v);
	}

	auto Vector::operator-(f32 v) const -> Vector {
		return Vector(x - v, y - v);
	}

	auto Vector::operator*(f32 v) const -> Vector {
		return Vector(x * v, y * v);
	}

	auto Vector::operator/(f32 v) const -> Vector {
		return Vector(x / v, y / v);
	}

	auto Vector::operator+(const Vector& v) const -> Vector {
		return Vector(x + v.x, y + v.y);
	}

	auto Vector::operator-(const Vector& v) const -> Vector {
		return Vector(x - v.x, y - v.y);
	}

	auto Vector::dot(Vector v) -> f32 {
		return x * v.x + y * v.y;
	}

	auto Vector::dot(f32 x, f32 y) -> f32 {
		return this->x * x + this->y * y;
	}

	auto Vector::lengthSquared() -> f32 {
		return x * x + y * y;
	}

	auto Vector::length() -> f32 {
		return sqrt(x * x + y * y);
	}

	auto Vector::angle() -> f32 {
		return atan2(y, x);
	}

	auto Vector::getX() -> f32 {
		return x;
	}

	auto Vector::getY() -> f32 {
		return y;
	}

	auto Vector::set(f32 x, f32 y) -> Vector& {
		this->x = x;
		this->y = y;
		return *this;
	}

	auto Vector::setX(f32 x) -> Vector& {
		this->x = x;
		return *this;
	}

	auto Vector::setY(f32 y) -> Vector& {
		this->y = y;
		return *this;
	}

	auto Vector::normalizeAngle(f32 a) -> Vector& {
		this->x = cos(a);
		this->y = sin(a);
		return *this;
	}

	auto Vector::setAngle(f32 a) -> Vector& {
		auto l = length();
		this->x = cos(a) * l;
		this->y = sin(a) * l;
		return *this;
	}

	auto Vector::rotate(f32 a) -> Vector& {
		auto sine = sin(a);
		auto cosine = cos(a);
		auto oldX = x;
		x = x * cosine - y * sine;
		y = oldX * sine + y * cosine;
		return *this;
	}

	auto Vector::negate() -> Vector& {
		x = -x;
		y = -y;
		return *this;
	}

	auto Vector::project(Vector v) -> Vector& {
		auto scalar = dot(v) / v.lengthSquared();
		x /= scalar;
		y /= scalar;
		return *this;
	}

	auto Vector::project(f32 x, f32 y) -> Vector& {
		auto scalar = dot(x, y) / (x * x + y * y);
		x /= scalar;
		y /= scalar;
		return *this;
	}

	auto Vector::normalize() -> Vector& {
		auto l = length();
		x /= l;
		y /= l;
		return *this;
	}

	auto Vector::setLength(f32 l) -> Vector& {
		auto scalar = l / length();
		x *= scalar;
		y *= scalar;
		return *this;
	}

	auto Vector::addX(f32 x) -> Vector & {
		this->x += x;
		return *this;
	}

	auto Vector::addY(f32 y) -> Vector & {
		this->y += y;
		return *this;
	}
}
